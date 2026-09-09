func makePrefixer = define(prefix) {
    func addPrefix = define(str) {
        return prefix + str;
    }
    return addPrefix;
}

func makeSuffixer = define(suffix) {
    func addSuffix = define(str) {
        return str + suffix;
    }
    return addSuffix;
}

func wrapInBorder = define(text) {
    let topBottom = "************************************";
    return topBottom + "\n* " + text + " *\n" + topBottom;
}

let logInfo = makePrefixer("[INFO] ");
let logError = makePrefixer("[ERROR] ");
let addTimestamp = makeSuffixer(" (Timestamp: 2026-07-25)");

output("--- Logging Pipeline ---");
let msg1 = logInfo(addTimestamp("System initialized successfully"));
output(msg1);

let msg2 = logError(addTimestamp("Failed to connect to remote server"));
output(msg2);

output("\n--- Formatted Banner ---");
let bannerText = "WELCOME TO FALX PROGRAMMING LANGUAGE";
output(wrapInBorder(bannerText));
