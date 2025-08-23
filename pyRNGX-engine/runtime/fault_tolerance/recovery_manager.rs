use std::time::Duration;

pub fn retry<F, T, E>(mut f: F, attempts: usize, backoff_ms: u64) -> Result<T, E>
where
    F: FnMut() -> Result<T, E>,
{
    let mut tries = 0;
    loop {
        match f() {
            Ok(v) => return Ok(v),
            Err(e) => {
                tries += 1;
                if tries >= attempts {
                    return Err(e);
                }
                std::thread::sleep(Duration::from_millis(backoff_ms));
            }
        }
    }
}
