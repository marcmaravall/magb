#ifndef MAGB_FRONTEND_H
#define MAGB_FRONTEND_H

class Frontend 
{

private:
    virtual void init    () = 0;
    virtual void update  () = 0;

public:
    virtual void Main(int argc, char* argv[]) = 0;

    Frontend() {}
    ~Frontend() {}
};

#endif // MAGB_FRONTEND_H
