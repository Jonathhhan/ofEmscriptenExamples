if type(window) ~= "userdata" then 
window = ofWindow() 
end 
 
local a = ofelia 
local canvas = ofCanvas(this) 
local clock = ofClock(this, "setup") 
local shaderDir = canvas:getDir() .. "/data/" 
local x, y, i, j, oldGridNumX, oldGridNumY = 0, 0, 0, 0, 0, 0 
local rectangle, color = ofDrawRectangle, ofSetColor 
local platform = ofGetTargetPlatform() 
a.shaderRaster = ofShader() 
a.fboCells = ofFbo() 
a.fboLines = ofFbo() 
a.gridNumX, a.gridNumY = 0, 0 
a.matrix = {} 
a.counter = 0 
a.labelFbo = ofFbo() 
 
function a.new() 
ofWindow.addListener("setup", this) 
ofWindow.addListener("draw", this) 
ofWindow.addListener("update", this) 
ofWindow.addListener("mousePressed", this) 
ofWindow.addListener("exit", this) 
window:setPosition(30, 100) 
window:setSize(840, 860) 
if ofWindow.exists then 
clock:delay(0) 
else
window:create() 
end 
end 
 
function a.free() 
window:destroy() 
ofWindow.removeListener("setup", this) 
ofWindow.removeListener("draw", this) 
ofWindow.removeListener("update", this) 
ofWindow.removeListener("mousePressed", this) 
ofWindow.removeListener("exit", this) 
end 
 
function a.setup()
ofSetBackgroundColor(100, 200, 255) 
a.width, a.height = ofGetWidth() - 40, (ofGetHeight() - 60)/1.5 
a.fboLines:allocate(a.width, a.height) 
a.shaderRaster:load(shaderDir .. "Raster") 
a.labelFbo:allocate(840, 860) 
end 
 
function a.update() 
if a.gridNumX ~= oldGridNumX or a.gridNumY ~= oldGridNumY then 
oldGridNumX = a.gridNumX 
oldGridNumY = a.gridNumY 
a.gridSize() 
end 
if a.setPattern == 1 then 
a.pattern() 
a.setPattern = 0 
end 
if a.cellsDraw == 1 then 
a.cells() 
a.cellsDraw = 0 
end 
end 
 
function a.draw() 
a.labelFbo:draw(0, 0) 
ofSetColor(20, 170, 150) 
ofDrawRectangle((a.counter * a.width/a.gridNumX) + 20, 20, a.width/a.gridNumX, a.height)
if a.fboCells:isAllocated() then
a.fboCells:getTexture():setTextureMinMagFilter(GL_NEAREST, GL_NEAREST) 
a.fboCells:draw(20, 20, a.width, a.height)
end
a.fboLines:draw(20, 20) 
return bang 
end 
 
function a.mousePressed(e)
if e.x > 20 and e.x < a.width + 20 and e.y > 20 and e.y < a.height + 20 and a.fboCells:isAllocated() then 
x = (e.x - 20 + (a.width / a.gridNumX)) // (a.width / a.gridNumX) 
y = (e.y - 20 + (a.height / a.gridNumY)) // (a.height / a.gridNumY) 
a.fboCells:beginFbo() 
ofDisableAlphaBlending() 
if a.matrix[x][y] == 1 then 
a.matrix[x][y] = 0 
color(255, 255, 255, 0) 
rectangle(x - 1, y - 1, 1, 1) 
else 
a.matrix[x][y] = 1 
color(0, 0, 0, 255) 
rectangle(x - 1, y - 1, 1, 1) 
end 
a.fboCells:endFbo() 
end 
end 
 
function a.gridSize() 
for x = 1, a.gridNumX do 
a.matrix[x] = {} 
for y = 1, a.gridNumY do 
a.matrix[x][y] = 0 
end 
end 
a.fboCells:allocate(a.gridNumX, a.gridNumY) 
a.shaderRaster:beginShader() 
a.shaderRaster:setUniformTexture("Tex0", a.fboLines:getTexture(), 1) 
a.shaderRaster:setUniform2f("resolution", a.width, a.height) 
a.shaderRaster:setUniform1f("gridNumX", a.gridNumX) 
a.shaderRaster:setUniform1f("gridNumY", a.gridNumY) 
a.fboLines:beginFbo() 
ofClear(0) 
ofDrawRectangle(0, 0, a.width, a.height) 
a.fboLines:endFbo() 
a.shaderRaster:endShader() 
end 
 
function a.pattern() 
for x = 1, a.gridNumX do 
a.matrix[x] = {} 
for y = 1, a.gridNumY do 
a.matrix[x][y] = 0 
end 
end 
if type(a.presetList) ~= "table" then 
a.presetList = {a.presetList} 
end 
a.fboCells:beginFbo() 
ofClear(0) 
color(0) 
for k = 1, #a.presetList do 
x = a.presetList[k] // a.gridNumY + 1 
y = a.presetList[k] % a.gridNumY + 1 
rectangle(x - 1, y - 1, 1, 1) 
a.matrix[x][y] = 1 
end 
a.fboCells:endFbo() 
end 
 
function a.Reset() 
a.fboCells:beginFbo() 
ofClear(0) 
a.fboCells:endFbo() 
for x = 1, a.gridNumX do 
a.matrix[x] = {} 
for y = 1, a.gridNumY do 
a.matrix[x][y] = 0 
end 
end 
end 
 
function a.cells() 
a.fboCells:beginFbo() 
ofClear(0) 
for x = 1, a.gridNumX do 
for y = 1, a.gridNumY do 
if a.matrix[x][y] == 1 then 
ofSetColor(0, 0, 0, 255) 
ofDrawRectangle(x - 1, y - 1, 1, 1) 
end 
end 
end 
a.fboCells:endFbo() 
end 
 
function a.exit() 
a.fboCells:clear() 
a.fboLines:clear() 
a.labelFbo:clear() 
a.shaderRaster:unload() 
end 


