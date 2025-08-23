pub struct RabbitConnector {
    url: String,
    connected: bool,
}

impl RabbitConnector {
    pub fn new(url: impl Into<String>) -> Self {
        Self { url: url.into(), connected: false }
    }
    pub fn connect(&mut self) -> bool {
        self.connected = true;
        true
    }
    pub fn publish(&self, queue: &str, msg: &str) -> bool {
        if !self.connected { return false; }
        println!("[rabbitmq] publish queue={} msg={}", queue, msg);
        true
    }
}
