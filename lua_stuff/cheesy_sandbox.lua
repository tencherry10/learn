
local env = {}

for i,f in ipairs({
  "print",
  "assert", "error", "ipairs", "next", 
  "pairs", "select", "tonumber", "tostring", "type", 
  "unpack", "_VERSION", "xpcall"}) 
do
  env[f]=_G[f]
end

local function run(untrusted_code)
  local untrusted_function, message = load(untrusted_code, nil, 't', env)
  if not untrusted_function then return nil, message end
  return pcall(untrusted_function)
end

assert(run([[
  print "hello world" 
  assert(1)
]]))
