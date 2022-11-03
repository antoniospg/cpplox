import sys
from cpp_builder import CppBuilder

ast_types = [
    ("Binary", [
        ("Expr<T>*", "left"),
        ("Token", "op"),
        ("Expr<T>*", "right")
    ]),
    ("Grouping", [
        ("Expr<T>*", "grouping")
    ]),
    ("Literal", [
        ("Obj", "value")
    ]),
    ("Unary", [
        ("Token", "op"),
        ("Expr<T>*", "right")
    ])
]

file_path = sys.argv[1]

base_name = "Expr"
builder = CppBuilder(file_path + "/" + "Expr.hpp", "EXPR_HPP")
builder.build_include("<string>")
builder.build_include("<memory>")
builder.build_include("\"../Lex.h\"")
builder.build_include("\"../Util.h\"")
builder.build_using_namespace("std")

builder.build_nl()
builder.build_forward(ast_types)
builder.build_forward([("Expr", [])])

builder.build_visitor("Expr", ast_types)

builder.build_nl()
builder.build_class(base_name, "", [])

for name, fields in ast_types:
    builder.build_nl()
    builder.build_class(name, "Expr", fields)

builder.gen()
