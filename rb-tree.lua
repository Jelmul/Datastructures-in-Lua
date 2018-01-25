local setmetatable = setmetatable

Node = {key = nil, parent = nil, color = nil, left = nil, right = nil}
Node.__index = Node

function Node.new(key)
    local node = {}
    setmetatable(node, Node)
    node.key = key
    return node
end

RBtree = {root = nil, size = 0}
RBtree.__index = RBtree

function RBtree.new()
    local tree = {}
    setmetatable(tree, RBtree)
    return tree
end

local RED = true
local BLACK = false

function RBtree:insert(i)
    local n = Node:new()
    n.key = i
    n.left = nil
    n.right = nil
    if self.root == nil then
        self.root = n
        n.color = BLACK
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
        n.color = RED
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
    local g
    local y
    while z ~= self.root and z.parent.color == RED do
        if z.parent == z.parent.parent.left then
            g = z.parent.parent
            y = g.right
            if y ~= nil and y.color == RED then
                z.parent.color = BLACK
                y.color = BLACK
                g.color = RED
                z = g
            else
                if z.parent.right == z then
                    z = z.parent
                    self:leftrotate(z)
                end

                z.parent.color = BLACK
                g.color = RED
                self:rightrotate(g)
            end
        else
            g = z.parent.parent
            y = g.left
            if y ~= nil and y.color == RED then
                z.parent.color = BLACK
                y.color = BLACK
                g.color = RED
                z = g
            else
                if z.parent.left == z then
                    z = z.parent
                    self:rightrotate(z)
                end

                z.parent.color = BLACK
                g.color = RED
                self:leftrotate(g)
            end
        end
    end
    self.root.color = BLACK
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
            if p.key < i then
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
    if y.color == BLACK then
        self:delfix(q)
    end
end

function RBtree:delfix(p)
    if p == nil then
        return
    end
    
    local s = nil
    while p~=nil and p ~= self.root and p.color == BLACK do
        if p.parent.left == p then
            s = p.parent.right
            if s~=nil and s.color == RED then
                s.color = BLACK
                p.parent.color = RED
                self:leftrotate(p.parent)
                s = p.parent.right
            end
            if s==nil then
                break
            end
            if (s.right == nil or s.right.color == BLACK) and (s.left == nil or s.left.color == BLACK) then
                s.color = RED
                p = p.parent
            else
                if s.right == nil or s.right.color == BLACK then
                    if s.left ~= nil then
                        s.left.color = BLACK
                    end
                    s.color = RED
                    self:rightrotate(s)
                    s = p.parent.right
                end
                s.color = p.parent.color
                p.parent.color = BLACK
                s.right.color = BLACK
                self:leftrotate(p.parent)
                p = self.root
            end
        else
            s = p.parent.left
            if s ~= nil and s.color == RED then
                s.color = BLACK
                p.parent.color = RED
                self:rightrotate(p.parent)
                s = p.parent.left
            end
            if s==nil then
                break
            end
            if (s.left == nil or s.left.color == BLACK) and (s.right == nil or s.right.color == BLACK) then
                s.color = RED
                p = p.parent
            else
                if s.left == nil or s.left.color == BLACK then
                    if s.right~= nil then
                        s.right.color = BLACK
                    end
                    s.color = RED
                    self:leftrotate(s)
                    s = p.parent.left
                end
                s.color = p.parent.color
                p.parent.color = BLACK
                if s.left ~= nil then
                    s.left.color = BLACK
                end
                self:rightrotate(p.parent)
                p = self.root
            end
        end
        p.color = BLACK
        self.root.color = BLACK
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
            self.root.parent = nil
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
        return false
    end
    local p = self.root
    local found = false
    while p ~= nil and not found do
        if p.key == x then
            return true
        end
        if not found then
            if p.key < x then
                p = p.right
            else
                p = p.left
            end
        end
    end
    return false
end