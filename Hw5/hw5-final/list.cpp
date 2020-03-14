void listAll(std::string path, const Class* c)  // two-parameter overload
{
    std::string lineOutput=path+c->name();
    if (path!="")
        lineOutput =path+"=>"+c->name();
    std::cout << lineOutput<<std::endl;
    for(std::vector<Class*>::const_iterator it =c->subclasses().begin();it!=c->subclasses().end();it++)
        listAll(lineOutput, *it);
}
