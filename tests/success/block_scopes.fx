let globalVar = "global";

{
    let blockVar = "block level 1";
    output(globalVar);
    output(blockVar);

    {
        let innerVar = "block level 2";
        output(innerVar);
        output(blockVar);
        output(globalVar);
    }
}

output(globalVar);
