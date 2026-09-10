func makeMultiplier = define(factor) {
    func multiplier = define(x) {
        return x * factor;
    }
    return multiplier;
}

let double = makeMultiplier(2);
let triple = makeMultiplier(3);

output("double 5: " + double(5));
output("triple 5: " + triple(5));

func applyTwice = define(f, x) {
    return f(f(x));
}

func addTen = define(val) {
    return val + 10;
}

output("applyTwice: " + applyTwice(addTen, 5));
