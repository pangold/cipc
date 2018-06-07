#ifndef __FACTORY_HPP__
#define __FACTORY_HPP__

#include <memory>
#include <string>
#include <map>

namespace ipc {
namespace utils {

template <typename Product>
class factory
{
public:
    typedef std::shared_ptr<Product> product_ptr;
    typedef std::map<std::string, product_ptr> product_map;

public:   
    template <typename ConcreteProduct>
    void add(const std::string& name)
    {
        _products[name] = product_ptr(new ConcreteProduct());
    }

    template <typename ConcreteProduct>
    void add(const std::string& name, ConcreteProduct& concrete)
    {
        _products[name] = product_ptr(concrete());
    }

    template <typename ConcreteProduct, typename... Args>
    void add(const std::string& name, const Args&... args)
    {
        _products[name] = product_ptr(new ConcreteProduct(args...));
    }

    void remove(const std::string& name)
    {
        auto it = _products.find(name);
        if (it != _products.end())
            _products.erase(it);
    }

    void clear()
    {
        _products.clear();
    }

    Product* instance(const std::string& name)
    {
        auto it = _products.find(name);
        if (it != _products.end())
            return it->second.get();
        return NULL;
    }

private:
    product_map _products;

};

} // namespace utils 
} // namespace ipc 

#endif // __FACTORY_H__

