import ast

class part1(ast.NodeTransformer):
    def visit_BinOp(self, node):
        self.generic_visit(node)
        if type(node.op) is ast.Sub:
            newnode = ast.BinOp(left=node.left, op=ast.Mult(), right=node.right)
            ast.copy_location(newnode, node)
            ast.fix_missing_locations(newnode)
            return newnode
        else:
            return node

class part2(ast.NodeTransformer):
    def visit_BinOp(self, node):
        self.generic_visit(node)
        if type(node.op) is ast.Sub:
            newnode = ast.BinOp(left=node.left, op=ast.Mult(), right=node.right)
            ast.copy_location(newnode, node)
            ast.fix_missing_locations(newnode)
            return newnode
        elif type(node.op) is ast.Div:
            newnode = ast.BinOp(left=node.left, op=ast.Add(), right=node.right)
            ast.copy_location(newnode, node)
            ast.fix_missing_locations(newnode)
            return newnode
        else:
            return node

# part 1
res = 0
with open("input.txt", "r") as f:
    for line in f:
        tree = ast.parse(line.replace("*", "-"), mode="eval")
        tree = part1().visit(tree)
        res += eval(compile(tree, filename="<ast>", mode="eval"))

print('part 1: %d'%res)

# part 2
res = 0
with open("input.txt", "r") as f:
    for line in f:
        tree = ast.parse(line.replace("*", "-").replace('+','/'), mode="eval")
        tree = part2().visit(tree)
        res += eval(compile(tree, filename="<ast>", mode="eval"))

print('part 2: %d'%res)
