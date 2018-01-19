Node = {}
Node.__index = Node

function Node.new(key)
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

function RBtree.new()
    local tree = {}
    setmetatable(tree, RBtree)
    tree.root = nil
    tree.size = 0
    return tree
end

function RBtree:insert(i)
    local n = Node:new()
    n.key = i
    if self.root == nil then
        self.root = n
        n.color = 'b'
        n.parent = nil
    else
        local p = self.root
        local parent = nil
        while p ~= nil do
            parent = p
            if p.key < i then
                p = p.right
            else
                p = p.left
            end
        end
        
        p = parent
        n.parent = p
        n.color = 'r'
        if p.key < n.key then
            p.right = n
        else
            p.left = n
        end
    end
    self.size = self.size + 1
    self:insertfix(n)
end

function RBtree:insertfix(z)
    while z ~= self.root and z.parent.color == 'r' do
        if z.parent == z.parent.parent.left then
            local g = z.parent.parent
            local y = g.right
            if y ~= nil and y.color == 'r' then
                z.parent.color = 'b'
                g.color = 'r'
                z = g
            else
                if z.parent.right == z then
                    z = z.parent
                    self:leftrotate(z)
                end

                z.parent.color = 'b'
                g.color = 'r'
                self:rightrotate(g)
            end
        else
            local g = z.parent.parent
            local y = g.left
            if y ~= nil and y.color == 'r' then
                z.parent.color = 'b'
                y.color = 'b'
                g.color = 'r'
                z = g
            else
                if z.parent.left == z then
                    z = z.parent
                    self:rightrotate(z)
                end

                z.parent.color = 'b'
                g.color = 'r'
                self:leftrotate(g)
            end
        end
    end
    self.root.color = 'b'
end

function RBtree:del(i)
    if self.root == nil then return end
    
    local p = self.root
    local y = nil
    local q = nil
    local found = false

    while p ~= nil and not found do
        if p.key == i then
            found = true
            self.size = self.size - 1
        end

        if not found then
            if p.key < i then -- go to correct child
                p = p.right
            else
                p = p.left
            end
        end
    end

    if not found then return end

    if p.left == nil or p.right == nil then
        y = p
    else
        y = self:successor(p)
    end
    if y.left ~= nil then
        q = y.left
    else
        if y.right ~= nil then
            q = y.right
        else
            q = nil
        end
    end
    if q ~= nil then
        q.parent = y.parent
    end
    if y.parent == nil then
        self.root = q
    else
        if y == y.parent.left then
            y.parent.left = q
        else
            y.parent.right = q
        end
    end
    if y ~= p then
        p.color = y.color
        p.key = y.key
    end
    if y.color == 'b' then
        self:delfix(q)
    end
end

function RBtree:delfix(p)
    if p == nil then
        return
    end
    
    local s = nil
    while p ~= self.root and p.color == 'b' do
        if p.parent.left == p then
            s = p.parent.right
            if s.color == 'r' then
                s.color = 'b'
                p.parent.color = 'r'
                self:leftrotate(p.parent)
                s = p.parent.right
            end
            if s.right.color == 'b' and s.left.color == 'b' then
                s.color = 'r'
                p = p.parent
            else
                if s.right.color == 'b' then
                    s.left.color = 'b'
                    s.color = 'r'
                    self:rightrotate(s)
                    s = p.parent.right
                end
                s.color = p.parent.color
                p.parent.color = 'b'
                s.right.color = 'b'
                self:leftrotate(p.parent)
                p = self.root
            end
        else
            s = p.parent.left
            if s.color == 'r' then
                s.color = 'b'
                p.parent.color = 'r'
                self:rightrotate(p.parent)
                s = p.parent.left
            end
            if s.left.color == 'b' and s.right.color == 'b' then
                s.color = 'r'
                p = p.parent
            else
                if s.left.color == 'b' then
                    s.right.color = 'b'
                    s.color = 'r'
                    self:leftrotate(s)
                    s = p.parent.left
                end
                s.color = p.parent.color
                p.parent.color = 'b'
                s.left.color = 'b'
                self:rightrotate(p.parent)
                p = self.root
            end
        end
        p.color = 'b'
        self.root.color = 'b'
    end
end

function RBtree:leftrotate(p)
    if p.right == nil then
        return
    else
        local y = p.right
        if y.left ~= nil then
            p.right = y.left
            y.left.parent = p
        else
            p.right = nil
        end
        if p.parent ~= nil then
            y.parent = p.parent
        end
        if p.parent == nil then
            self.root = y
            self.root.parent = nil -- fix
        else
            if p == p.parent.left then
                p.parent.left = y
            else
                p.parent.right = y
            end
        end
        y.left = p
        p.parent = y
    end

end

function RBtree:rightrotate(p)
    if p.left == nil then
        return
    else
        local y = p.left
        if y.right ~= nil then
            p.left = y.right
            y.right.parent = p
        else
            p.left = nil
        end
        if p.parent ~= nil then
            y.parent = p.parent
        end
        if p.parent == nil then
            self.root = y
            y.parent = nil -- fix
        else
            if p == p.parent.left then
                p.parent.left = y
            else
                p.parent.right = y
            end
        end
        y.right = p
        p.parent = y
    end
end

function RBtree:successor(p)
    local y = nil
    if p.left ~= nil then
        y = p.right
        while y.left ~= nil do
            y = y.left
        end
    else
        y = p.left
        while y.right ~= nil do
            y = y.right
        end
    end
    return y
end

function RBtree:search(x)
    if self.root == nil then
      print('root == nil')
        return false
    end
    local p = self.root
    local found = false
    while p ~= nil and not found do
        if p.key == x then
            return true
        end
        if not found then
            if p.key < x then -- go to correct child
                p = p.right
            else
                p = p.left
            end
        end
    end
    --print(string.format("%s not found", x))
    return false
end

function RBtree:print(p, i)
    if p ~= nil and i >= 0 then
        local left = -1
        local right = -1
        if p.left ~= nil then left = p.left.key end
        if p.right ~= nil then right = p.right.key end
        print(string.format("%s has %s left and %s right", p.key, left, right))
        self:print(p.left, i - 1)
        self:print(p.right, i - 1)
    end
end

function RBtree:debug(p)
    local left = 0
    local right = 0
    if p.left ~= nil then
        left = self:children(p.left, 0)
    end
    if p.right ~= nil then
        right = self:children(p.right, 0)
    end
    print(string.format("%s has %s children left and %s children right", p.key, left, right))
end

function RBtree:children(p, i)
    if i < 100 then
      local count = 1
      if p.left ~= nil then
          count = count + self:children(p.left, i + 1)
      end
      if p.right ~= nil then
          count = count + self:children(p.right, i + 1)
      end
      return count
    else
      print("   Enabling stack overflow exception")
      return count
    end
end

function RBtree:depth(p)
    local left = 0
    local right = 0
    if p.left ~= nil then
        left = self:depth(p.left)
    end
    if p.right ~= nil then
        right = self:depth(p.right)
    end
    if left > right then
        return 1 + left
    else
        return 1 + right
    end
end

--[[

print("Insert")
local tree = RBtree:new()
for i = 0, 100, 1 do
    tree:insert(i)
end

tree:debug(tree.root)

print("Simple test left")
current = tree.root
for i = 0, 10, 1 do
  if current ~= nil then
      print(current.key)
      current = current.left
  end
end
print("Simple test right")
current = tree.root
for i = 0, 10, 1 do
    if current ~= nil then
      print(current.key)
      current = current.right
    end
end

--tree:print(tree.root, 100)
print("Depth:")
print(tree:depth(tree.root))

print("Search existing")
for i = 1, 100, 10 do
    print(tree:search(i))
end

print("Del")
tree:debug(tree.root)
for i = 1, 100, 10 do
    tree:del(i)
end
tree:debug(tree.root)

print("Search deleted")
for i = 1, 100, 10 do
    print(tree:search(i))
end

print("Done")
]]
