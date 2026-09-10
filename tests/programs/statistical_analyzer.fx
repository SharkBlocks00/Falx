func calculateWeightedAverage = define(val1, w1, val2, w2, val3, w3) {
    let totalWeight = w1 + w2 + w3;
    let weightedSum = val1 * w1 + val2 * w2 + val3 * w3;
    return weightedSum / totalWeight;
}

func normalizeScore = define(score, maxScore) {
    let ratio = score / maxScore;
    return ratio * 100;
}

func computeVarianceStep = define(val, mean) {
    let diff = val - mean;
    return diff * diff;
}

output("=== STATISTICAL ANALYZER ===");

let exam1 = 85.0;
let exam2 = 92.0;
let finalExam = 78.0;

let normExam1 = normalizeScore(exam1, 100.0);
let normExam2 = normalizeScore(exam2, 100.0);
let normFinal = normalizeScore(finalExam, 100.0);

output("Normalized Scores:");
output("Exam 1: " + normExam1 + "%");
output("Exam 2: " + normExam2 + "%");
output("Final Exam: " + normFinal + "%");

let finalGrade = calculateWeightedAverage(normExam1, 0.25, normExam2, 0.25, normFinal, 0.50);
output("Final Course Grade: " + finalGrade + "%");

let v1 = computeVarianceStep(normExam1, finalGrade);
let v2 = computeVarianceStep(normExam2, finalGrade);
let v3 = computeVarianceStep(normFinal, finalGrade);
let variance = v1 + v2 + v3 / 3;

output("Variance Metric: " + variance);
output("=============================");
