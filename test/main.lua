local utils = require("utils")

local num = 0
for i, v in ipairs(utils.nums) do
    num = num + v
end

print(num)
