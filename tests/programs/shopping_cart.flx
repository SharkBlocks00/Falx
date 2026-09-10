func createDiscountCalculator = define(discountRate) {
    func applyDiscount = define(amount) {
        let discountAmount = amount * discountRate;
        return amount - discountAmount;
    }
    return applyDiscount;
}

func calculateTax = define(subtotal, taxRate) {
    return subtotal * taxRate;
}

let vipDiscount = createDiscountCalculator(0.20);
let regularDiscount = createDiscountCalculator(0.05);

const taxRate = 0.08;

output("=================================");
output("   SHOPPING CART RECEIPT SYSTEM  ");
output("=================================");

let item1Price = 49.99;
let item2Price = 120.00;
let item3Price = 15.50;

let rawSubtotal = item1Price + item2Price + item3Price;
output("Raw Subtotal: $" + rawSubtotal);

let vipSubtotal = vipDiscount(rawSubtotal);
output("VIP Discounted Subtotal: $" + vipSubtotal);

let tax = calculateTax(vipSubtotal, taxRate);
output("Tax (8%): $" + tax);

let finalTotal = vipSubtotal + tax;
output("Final Total: $" + finalTotal);
output("=================================");
