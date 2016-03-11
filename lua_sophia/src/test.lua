register_sophia = package.loadlib("./libluasophia.so", "register_sophia")
r,e = register_sophia(1,0,0)

function sophia.opendb(path, dbname, env)
  if(not env) then
    env, e  = sophia.env() ; assert(not e)
    r,e     = sophia.setstring(env, "sophia.path", path) ; assert(not e)
    r,e     = sophia.setstring(env, "db", dbname) ; assert(not e)
    r,e     = sophia.open(env) ; assert(not e)    
  end
  db, e   = sophia.getobject(env, "db."..dbname) ; assert(not e)
  return env, db
end

function sophia.write(db, key, value, sz)
  sz      = sz or 0
  doc,e   = sophia.document(db) ; assert(not e)
  r,e     = sophia.setstring(doc, "key", key) ; assert(not e)
  r,e     = sophia.setstring(doc, "value", value, sz) ; assert(not e)
  r,e     = sophia.set(db, doc) ; assert(not e)
  return nil,nil
end

function sophia.read(db, key)
  doc,e   = sophia.document(db) ; assert(not e)
  r,e     = sophia.setstring(doc, "key", key) ; assert(not e)
  doc, e  = sophia.get(db, doc) ; assert(not e)
  v, e,sz = sophia.getstring(doc, "value") ; assert(not e)
  r,e     = sophia.destroy(doc) ; assert(not e)
  return v, sz
end


env, db = sophia.opendb("_test", "test")
r, e    = sophia.write(db, "hello", "world")
res, sz = sophia.read(db, "hello")
print(res, sz)



doc, e  = sophia.document(db) ; assert(not e)
r,e     = sophia.setstring(doc, "key", "1") ; assert(not e)
doc, e  = sophia.get(db, doc) ; assert(not e)
r,e     = sophia.getstring(doc, "key") ; assert(not e)
print(r)  
r,e     = sophia.destroy(doc) ; assert(not e)

doc, e  = sophia.document(db) ; assert(not e)
r, e    = sophia.setstring(doc, "key", "1") ; assert(not e)
r, e    = sophia.delete(db, doc) ; assert(not e)

r,e     = sophia.destroy(env) ; assert(not e)
