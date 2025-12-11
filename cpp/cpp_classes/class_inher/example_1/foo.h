#ifndef FOO_H
#define FOO_H

template<typename T>
class container
{
    T a;
    public:
    container();
    container(T a);
    void who();
};


template<typename T>
class container_child: public container<T>
{
    public:
    container_child();
    container_child(T a);

};

#endif