import itertools

class CppBuilder:
    def __init__(self, path, guard):
        self.path = path
        self.src = ""
        self.guard = guard

        # Add header guard
        self.append_src("#ifndef " + self.guard)
        self.append_src("#define " + self.guard)

    def append_src(self, line):
        self.src = self.src + line + "\n"

    def add_ws(self, num):
        self.src = self.src + "".join(itertools.repeat(" ", num))

    def build_nl(self):
        self.append_src("")

    def build_include(self, header):
        new_str = "#include " + header
        self.append_src(new_str)

    def build_using_namespace(self, namespace):
        new_str = "using namespace " + namespace + ";"
        self.append_src(new_str)

    def build_template_header(self, name):
        new_str = "template <typename " + name + ">"
        self.append_src(new_str)

    def build_forward(self, types):
        for type_name, _ in types:
            self.build_template_header("T")
            self.append_src("class " + type_name + ";")
            self.append_src("")

    def build_class(self, name, parent, fields):
        # Class header
        self.build_template_header("T")
        if (parent == ""):
            self.append_src("class " + name + " {")
        else:
            self.append_src("class " + name + " : public " + parent + "<T> {")

        # Public fields
        self.add_ws(1)
        self.append_src(" public:")

        # Gen class attributes
        for field_type, field_name in fields:
            self.add_ws(2)
            self.append_src(field_type + " " + field_name + ";")

        # Gen constructor
        self.add_ws(2)
        constructor = name + "( "
        for i, (field_type, field_name) in enumerate(fields):
            constructor = constructor + field_type + " " + field_name
            if (i < len(fields)-1):
                constructor = constructor + ", "
        if len(fields) > 0:
            constructor = constructor + ") : "
        else:
            constructor = constructor + ") "

        # Intizalize fields
        for i, (field_type, field_name) in enumerate(fields):
            constructor = constructor + field_name + "(" + field_name + ")"
            if (i < len(fields)-1):
                constructor = constructor + ", "

        constructor = constructor + " {}"
        self.append_src(constructor)

        # Visitor methods
        self.add_ws(2)
        # Check if it's base class or not
        if parent == "":
            self.append_src("virtual T accept (" + name + "AstVisitor<T>*" + " visitor) = 0;")
        else:
            self.append_src("T accept (" + parent + "AstVisitor<T>*" + " visitor) {")
            self.add_ws(4)
            self.append_src("return visitor->visit"  + parent + name + "(this);")
            self.add_ws(2)
            self.append_src("}")

        self.append_src("};")

    def build_visitor(self, base_name, ast_types):
        self.build_template_header("T")

        # Header
        self.append_src("class " + base_name + "AstVisitor {")
        self.append_src("public:")

        # Methods
        for type_name, _ in ast_types:
            self.add_ws(2)
            new_str = "virtual T visit" + base_name + type_name + " (" + type_name + \
                "<T>* " + base_name.lower() + ") = 0;"
            self.append_src(new_str)
        self.append_src("};")


    def gen(self):
        self.append_src("#endif")

        f = open(self.path, "w", encoding='utf-8')
        f.write(self.src)
