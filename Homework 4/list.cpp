void listAll(const Class* c, string path)  // two-parameter overload
{
    if (path == "")
        path = c->name();      // initially set path to name of base class
    
    std::cout << path << std::endl;
    
    vector<Class*>::const_iterator it;
    it = c->subclasses().begin();
    
    while (it != c->subclasses().end()) {
        listAll(*it, path + "=>" + (*it)->name());
        it++;
    }
}
