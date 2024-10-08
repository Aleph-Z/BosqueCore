"use strict";

import { runMainCode } from "../../../bin/test/runtime/runtime_nf.js";
import { describe, it } from "node:test";

describe ("Exec -- NamespaceFunction (no template)", () => {
    it("should exec simple positional", function () {
        runMainCode("function foo(): Int { return 1i; } public function main(): Int { return foo(); }", [1n, "Int"]);
        runMainCode("function foo(x: Int, y: Bool): Int { return x; } public function main(): Int { return foo(1i, true); }", [1n, "Int"]);
    });

    it("should exec simple named", function () {
        runMainCode("function foo(x: Int, y: Bool): Int { return x; } public function main(): Int { return foo(x=1i, y=true); }", [1n, "Int"]);
        runMainCode("function foo(x: Int, y: Bool): Int { return x; } public function main(): Int { return foo(y=true, x=1i); }", [1n, "Int"]);
    });

    it("should exec simple mixed", function () {
        runMainCode("function foo(x: Int, y: Bool): Int { return x; } public function main(): Int { return foo(x=1i, true); }", [1n, "Int"]);
        runMainCode("function foo(x: Int, y: Bool): Int { return x; } public function main(): Int { return foo(y=true, 1i); }", [1n, "Int"]);
    });
});
