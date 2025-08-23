// Stateless executor facade over TaskScheduler.
use crate::scheduler::task_scheduler::TaskScheduler;
use std::sync::Arc;

pub struct Executor {
    scheduler: Arc<TaskScheduler>,
}

impl Executor {
    pub fn new(workers: usize) -> Self {
        Self { scheduler: Arc::new(TaskScheduler::new(workers)) }
    }

    pub fn submit<F>(&self, f: F)
    where
        F: FnOnce() + Send + 'static,
    {
        self.scheduler.submit(f);
    }

    pub fn shutdown(self) {
        Arc::try_unwrap(self.scheduler).ok().unwrap().shutdown();
    }
}
