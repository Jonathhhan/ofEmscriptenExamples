local a = require("$0-main") 
 
function ofelia.Shift_Left() 
for x = 1, a.gridNumX-1 do 
for y = 1, a.gridNumY do 
a.matrix[(x)%a.gridNumX+1][y], a.matrix[x][y] = a.matrix[x][y], a.matrix[(x)%a.gridNumX+1][y] 
end 
end 
a.cellsDraw = 1 
end 
 
function ofelia.Shift_Right() 
for x = 1, a.gridNumX-1 do 
for y = 1, a.gridNumY do 
a.matrix[((x * - 1) + a.gridNumX) % a.gridNumX + 1][y], a.matrix[((x * - 1) + a.gridNumX)][y] = a.matrix[((x*-1) + a.gridNumX)][y], a.matrix[((x * - 1) + a.gridNumX) % a.gridNumX + 1][y] 
end 
end 
a.cellsDraw = 1 
end 
 
function ofelia.Shift_Up() 
for x = 1, a.gridNumX do 
for y = 1, a.gridNumY-1 do 
a.matrix[x][y], a.matrix[x][(y)%a.gridNumY+1] = a.matrix[x][y%a.gridNumY+1], a.matrix[x][y] 
end 
end 
a.cellsDraw = 1 
end 
 
function ofelia.Shift_Down() 
for x = 1, a.gridNumX do 
for y = 1, a.gridNumY-1 do 
a.matrix[x][((y * - 1) + a.gridNumY) % a.gridNumY + 1], a.matrix[x][((y * - 1) + a.gridNumY)%a.gridNumY] = a.matrix[x][((y * - 1) + a.gridNumY)%a.gridNumY], a.matrix[x][((y * - 1) + a.gridNumY) % a.gridNumY + 1] 
end 
end 
a.cellsDraw = 1 
end 
 
function ofelia.Flip_X() 
for y = 1, a.gridNumY do 
local i, j = 1, a.gridNumX 
while i < j do 
a.matrix[i][y], a.matrix[j][y] = a.matrix[j][y], a.matrix[i][y] 
i = i + 1 
j = j - 1 
end 
end 
a.cellsDraw = 1 
end 
 
function ofelia.Flip_Y() 
for x = 1, a.gridNumX do 
local i, j = 1, a.gridNumY 
while i < j do 
a.matrix[x][i], a.matrix[x][j] = a.matrix[x][j], a.matrix[x][i] 
i = i + 1 
j = j - 1 
end 
end 
a.cellsDraw = 1 
end 
 
function ofelia.Shuffle() 
for i = 1, a.gridNumX do 
for j = 1, a.gridNumY do 
local x = math.random(i) 
local y = math.random(j) 
a.matrix[i][j], a.matrix[x][y] = a.matrix[x][y], a.matrix[i][j] 
end 
end 
a.cellsDraw = 1 
end 
 
function ofelia.Shuffle_X() 
for i = 1, a.gridNumX do 
for j = 1, a.gridNumY do 
local x = math.random(i) 
local y = j 
a.matrix[i][y], a.matrix[x][y] = a.matrix[x][y], a.matrix[i][y] 
end 
end 
a.cellsDraw = 1 
end 
 
function ofelia.Shuffle_Y() 
for i = 1, a.gridNumX do 
for j = 1, a.gridNumY do 
local x = i 
local y = math.random(j) 
a.matrix[x][j], a.matrix[x][y] = a.matrix[x][y], a.matrix[x][j] 
end 
end 
a.cellsDraw = 1 
end 
 
function ofelia.Invert() 
for x = 1, a.gridNumX do 
for y = 1, a.gridNumY do 
if a.matrix[x][y] == 0 then 
a.matrix[x][y] = 1 
elseif a.matrix[x][y] > 0 then 
a.matrix[x][y] = 0 
end 
end 
end 
a.cellsDraw = 1 
end 
 
function ofelia.Reset() 
for x = 1, a.gridNumX do 
for y = 1, a.gridNumY do 
a.matrix[x][y] = 0 
end 
end 
a.cellsDraw = 1 
end 

