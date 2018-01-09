Node = {}
Node.__index = Node

function Node:new(key)
    local node = {}
    setmetatable(node, Node)
    node.key = key
    node.parent = nil
    node.color = nil
    node.left = nil
    node.right = nil
    return node
end

RBtree = {}
RBtree.__index = RBtree

function RBtree:new()
    local tree = {}
    setmetatable(tree, RBtree)
    tree.root = nil
    tree.size = 0
    return tree
end

function RBtree:insert(i)
    tree.size = i
    print(tree.size)
end

function RBtree:insertfix(z)
end

function RBtree:del(i)
end

function RBtree:delfix(p)
end

function RBtree:leftrotate(p)
end

function RBtree:rightrotate(p)
end

function RBtree:successor(p)
end

function RBtree:search(x)
end

print("Starting")
tree = RBtree:new()
for i = 0, 100, 1
do
    tree:insert(i)
end
print("Done")