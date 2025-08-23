use std::collections::HashMap;

fn main() {
    let input = "a a b c a b";
    let mut map: HashMap<&str, usize> = HashMap::new();
    for w in input.split_whitespace() {
        *map.entry(w).or_insert(0) += 1;
    }
    for (k, v) in map.iter() {
        println!("{}\t{}", k, v);
    }
}
