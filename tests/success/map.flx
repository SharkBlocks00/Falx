func t = define() {
    output("T Called!");
}

let map = {
    llava: "A",
    llama: "B",
    gpt: "C",
    test: t
};

output([].isEmpty());

output(map.size);

output(map.isEmpty());

map["name"] = "Test";
output(map.name);

map.test();

output(map.contains("llava"));
output(map.contains("missing"));

let copy = map.copy();
output(copy.size);

map.remove("gpt");
output(map.contains("gpt"));
output(map.size);

let keys = map.keys();
output(keys.size);

let values = map.values();
output(values.size);

let entries = map.entries();
output(entries.size);
map.clear();
output(map.size);
output(map.isEmpty());