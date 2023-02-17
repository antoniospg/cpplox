import sys
from cpp_builder import CppBuilder

expr_ast_types = [
    ("Binary", [
        ("Expr<T> *", "left"),
        ("Token", "op"),
        ("Expr<T> *", "right")
    ]),
    ("Call", [
        ("Expr<T> *", "callee"),
        ("Token", "paren"),
        ("list<Expr<T> *>", "arguments")
    ]),
    ("Grouping", [
        ("Expr<T> *", "grouping")
    ]),
    ("Literal", [
        ("Obj", "value")
    ]),
    ("Logical", [
        ("Expr<T> *", "left"),
        ("Token", "op"),
        ("Expr<T> *", "right")
    ]),
    ("Unary", [
        ("Token", "op"),
        ("Expr<T> *", "right")
    ]),
    ("Variable", [
        ("Token", "name")
    ]),
    ("Assign", [
        ("Token", "name"),
        ("Expr<T> *", "value")
    ])
]

stmt_ast_types = [
    ("Expression", [
        ("Expr<T> *", "expr")
    ]),
    ("Function", [
        ("Token", "name"),
        ("list<Token>", "params"),
        ("list<Stmt<T> *>", "body"),
    ]),
    ("If", [
        ("Expr<T> *", "condition"),
        ("Stmt<T> *", "thenBranch"),
        ("Stmt<T> *", "elseBranch")
    ]),
    ("Print", [
        ("Expr<T> *", "expr")
    ]),
    ("Return", [
        ("Token", "keyword"),
        ("Expr<T> *", "value")
    ]),
    ("Var", [
        ("Token", "name"),
        ("Expr<T> *", "initializer"),
    ]),
    ("While", [
        ("Expr<T> *", "condition"),
        ("Stmt<T> *", "body")
    ]),
    ("Block", [
        ("list<Stmt<T> *>", "statements")
    ])
]

file_path = sys.argv[1]

# Expr files
base_name = "Expr"
builder = CppBuilder(file_path + "/" + "Expr.hpp", "EXPR_HPP")
builder.build_include("<string>")
builder.build_include("<memory>")
builder.build_include("<list>")
builder.build_include("\"../Lex.h\"")
builder.build_include("\"../Util.h\"")
builder.build_using_namespace("std")

builder.build_nl()
builder.build_forward(expr_ast_types)
builder.build_forward([("Expr", [])])

builder.build_visitor("Expr", expr_ast_types)

builder.build_nl()
builder.build_class(base_name, "", [])

for name, fields in expr_ast_types:
    builder.build_nl()
    builder.build_class(name, "Expr", fields)

builder.gen()

# Stmt files
base_name = "Stmt"
builder = CppBuilder(file_path + "/" + "Stmt.hpp", "STMT_HPP")
builder.build_include("<string>")
builder.build_include("<memory>")
builder.build_include("<list>")
builder.build_include("\"../Lex.h\"")
builder.build_include("\"../Util.h\"")
builder.build_include("\"Expr.hpp\"")
builder.build_using_namespace("std")

builder.build_nl()
builder.build_forward(stmt_ast_types)
builder.build_forward([("Stmt", [])])

builder.build_visitor("Stmt", stmt_ast_types)

builder.build_nl()
builder.build_class(base_name, "", [])

for name, fields in stmt_ast_types:
    builder.build_nl()
    builder.build_class(name, "Stmt", fields)

builder.gen()
