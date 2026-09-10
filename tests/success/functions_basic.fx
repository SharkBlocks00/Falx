func greet = define() {
    output("Hello from greet!");
}

greet();

func add = define(a, b) {
    return a + b;
}

let sum = add(10, 25);
output("sum: " + sum);

func calculate = define(a, b, c) {
    let temp = a * b;
    return temp - c;
}

output("calc: " + calculate(5, 4, 3));
