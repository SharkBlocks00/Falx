func add = define(a, b) { return a + b; }
func subtract = define(a, b) { return a - b; }
func multiply = define(a, b) { return a * b; }
func divide = define(a, b) { return a / b; }

func createScaledOp = define(op, scale) {
    func scaled = define(x, y) {
        let raw = op(x, y);
        return raw * scale;
    }
    return scaled;
}

let scaledAdd = createScaledOp(add, 2);
let scaledMultiply = createScaledOp(multiply, 0.5);

output("--- Calculator Engine Test ---");
output("10 + 5 = " + add(10, 5));
output("10 - 5 = " + subtract(10, 5));
output("10 * 5 = " + multiply(10, 5));
output("10 / 5 = " + divide(10, 5));

output("Scaled Add (10 + 5) * 2 = " + scaledAdd(10, 5));
output("Scaled Multiply (10 * 5) * 0.5 = " + scaledMultiply(10, 5));

{
    let x = 100;
    let y = 25;
    let result = add(multiply(x, y), divide(x, y));
    output("Complex expression ((100 * 25) + (100 / 25)) = " + result);
}
