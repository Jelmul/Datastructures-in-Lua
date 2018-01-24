-- Set up
n = 7

-- Priority queue class
PriorityQueue = {}
PriorityQueue.__index = PriorityQueue
function PriorityQueue:create(array)
  local prioq = {}
  setmetatable(prioq, PriorityQueue)
  make_heap(array)
  prioq.array = array
  return prioq
end

function PriorityQueue:push(k)
  push_heap(self.array, k)
end

function PriorityQueue:pop()
  return pop_heap(self.array)
end

function PriorityQueue:top()
  return peak_heap(self.array)
end

-- Helper functions
function divide(a,b)
    return (a - a % b) / b
end

function swap(array, a, b)
    temp = array[a]
    array[a] = array[b]
    array[b] = temp
end

function parent(i)
    return divide(i, 2)
end

function left(i)
    return(2 * i)
end

function right(i)
    return (2 * i) + 1
end

function sift_up(array, i)
    if(i == 1 or array[i] <= array[parent(i)]) then
        return
    end
    swap(array, i, parent(i))
    sift_up(array, parent(i))
end

function sift_down(array, i)
    j = i
    if left(i) <= n and array[left(i)] > array[j] then
        j = left(i)
    end
    if right(i) <= n and array[right(i)] > array[j] then
        j = right(i)
    end
    if j ~= i then
        swap(array, i, j)
        sift_down(array, j)
    end
end    

-- Heap functions
function make_heap(array)
    p = divide(n, 2)
    while p ~= 0 do
        sift_down(array, p)
        p = p - 1
    end
end

function push_heap(array, k)
    n = n + 1
    array[n] = k
    sift_up(array, n)
end

function pop_heap(array)
    element = array[1]
    array[1] = array[n]
    array[n] = nil
    n = n - 1
    sift_down(array, 1)
    return element
end

function peak_heap(array)
    return array[1]
end
    
-- Testing
--[[
a = {3, 20, 11, 15, 33, 2, 50}
test = PriorityQueue:create(a)
print(test:top())
print(test:pop())
test:push(10000)
print(test:top())
print(test:pop())
print("test")
]]
