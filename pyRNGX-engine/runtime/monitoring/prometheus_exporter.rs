use std::sync::atomic::{AtomicU64, Ordering};

pub static TASKS_COMPLETED: AtomicU64 = AtomicU64::new(0);

pub fn inc_tasks(n: u64) {
    TASKS_COMPLETED.fetch_add(n, Ordering::Relaxed);
}

pub fn scrape() -> String {
    let v = TASKS_COMPLETED.load(Ordering::Relaxed);
    format!(
        "# TYPE fastflow_tasks_completed counter\nfastflow_tasks_completed {}\n",
        v
    )
}
