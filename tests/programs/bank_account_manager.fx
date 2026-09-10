func createAccount = define(accountHolder, initialBalance) {
    let balance = initialBalance;

    func deposit = define(amount) {
        balance = balance + amount;
        output("Deposited $" + amount + " into " + accountHolder + "'s account. New Balance: $" + balance);
        return balance;
    }

    func withdraw = define(amount) {
        balance = balance - amount;
        output("Withdrew $" + amount + " from " + accountHolder + "'s account. New Balance: $" + balance);
        return balance;
    }

    func applyInterest = define(rate) {
        let interest = balance * rate;
        balance = balance + interest;
        output("Applied interest of $" + interest + " to " + accountHolder + "'s account. New Balance: $" + balance);
        return balance;
    }

    return deposit;
}

output("--- Bank Account Management System ---");
let depositAlice = createAccount("Alice", 500.0);
depositAlice(150.0);
depositAlice(200.0);
depositAlice(50.50);

let depositBob = createAccount("Bob", 1000.0);
depositBob(250.0);
depositBob(500.0);

output("Account operations completed.");
