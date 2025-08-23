// Threaded task scheduler (simple MPMC via Mutex). Replace with a lock-free queue later.
use std::collections::VecDeque;
use std::sync::{Arc, Mutex};
use std::thread;
use std::time::Duration;

pub type Task = Box<dyn FnOnce() + Send + 'static>;

pub struct TaskScheduler {
    queue: Arc<Mutex<VecDeque<Task>>>,
    workers: Vec<thread::JoinHandle<()>>,
    stop: Arc<Mutex<bool>>,
}

impl TaskScheduler {
    pub fn new(num_workers: usize) -> Self {
        let queue = Arc::new(Mutex::new(VecDeque::new()));
        let stop = Arc::new(Mutex::new(false));
        let mut workers = Vec::with_capacity(num_workers);

        for _ in 0..num_workers {
            let q = queue.clone();
            let s = stop.clone();
            workers.push(thread::spawn(move || loop {
                if *s.lock().unwrap() {
                    break;
                }
                let job_opt = q.lock().unwrap().pop_front();
                if let Some(job) = job_opt {
                    job();
                } else {
                    thread::sleep(Duration::from_millis(1));
                }
            }));
        }

        Self { queue, workers, stop }
    }

    pub fn submit<F>(&self, f: F)
    where
        F: FnOnce() + Send + 'static,
    {
        self.queue.lock().unwrap().push_back(Box::new(f));
    }

    pub fn shutdown(self) {
        *self.stop.lock().unwrap() = true;
        for h in self.workers {
            let _ = h.join();
        }
    }
}

#[cfg(test)]
mod tests {
    use super::*;
    use std::sync::atomic::{AtomicUsize, Ordering};
    use std::sync::Arc;

    #[test]
    fn it_runs_tasks() {
        let scheduler = TaskScheduler::new(2);
        let c = Arc::new(AtomicUsize::new(0));
        for _ in 0..8 {
            let c2 = c.clone();
            scheduler.submit(move || {
                c2.fetch_add(1, Ordering::SeqCst);
            });
        }
        std::thread::sleep(std::time::Duration::from_millis(50));
        scheduler.shutdown();
        assert!(c.load(Ordering::SeqCst) >= 8);
    }
}
