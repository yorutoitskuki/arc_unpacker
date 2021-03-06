#pragma once

#include <memory>
#include "types.h"

namespace au {
namespace algo {
namespace crypt {

    class Blowfish final
    {
    public:
        Blowfish(const bstr &key);
        ~Blowfish();
        static size_t block_size();
        void decrypt_in_place(bstr &input) const;
        bstr decrypt(const bstr &input) const;
        bstr encrypt(const bstr &input) const;

    private:
        struct Priv;
        std::unique_ptr<Priv> p;
    };

} } }
