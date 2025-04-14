template<int N>
class spc_coord {
private:
    using int_t = int64_t;
    using uint_t = uint64_t;
    static constexpr int PARTS = N;
    static constexpr int STRIDE_BITS = sizeof(uint_t) * 8;

    int_t high;
    uint_t lows[PARTS - 1];

public:
    constexpr spc_coord() : high(0), lows{0} {}

    template<typename... Args>
    constexpr spc_coord(int_t h, Args... nums) : high(h), lows{static_cast<uint_t>(nums)...} {}

    constexpr inline double to_double() const {
        bool sign = (high < 0);
        double result = 0.0;

        if (sign){
            const uint_t am = std::numeric_limits<uint_t>::max() - high;
            result = static_cast<double>(am) * std::pow(2.0, STRIDE_BITS * (PARTS - 1));
            for (int i = PARTS-2; i >=0; --i) {
                const uint_t am = std::numeric_limits<uint_t>::max() - lows[i];
                result += static_cast<double>(am) * std::pow(2.0, STRIDE_BITS * (PARTS - i - 2));
            }
            result = (result * -1.0) - 1.0;
        }else{
            result = static_cast<double>(high) * std::pow(2.0, STRIDE_BITS * (PARTS - 1));
            for (int i = 0; i < PARTS - 1; ++i) {
                const uint_t am = lows[i];
                result += static_cast<double>(am) * std::pow(2.0, STRIDE_BITS * (PARTS - i - 2));
            }
        }

        return result;
    }

    constexpr inline spc_coord& from_double(double value) {
        // if (value < static_cast<double>(std::numeric_limits<int_t>::min()) * std::pow(2.0, STRIDE_BITS * (PARTS - 1)) ||
        //     value > static_cast<double>(std::numeric_limits<int_t>::max()) * std::pow(2.0, STRIDE_BITS * (PARTS - 1))) {
        //     throw std::overflow_error("Value out of range for spc_coord");
        // }

        bool sign = (value < 0);
        high = static_cast<int_t>(value / std::pow(2.0, STRIDE_BITS * (PARTS - 1)));
        value -= static_cast<double>(high) * std::pow(2.0, STRIDE_BITS * (PARTS - 1));
        high -= sign ? 1 : 0;

        if (sign){
            for (int i = PARTS-2; i >=0; --i) {
                double partValue = std::floor(value / std::pow(2.0, STRIDE_BITS * (PARTS - i - 2)));
                lows[i] = static_cast<uint_t>(partValue);
                value += partValue * std::pow(2.0, STRIDE_BITS * (PARTS - i - 2));
            }
        }else{
            for (int i = 0; i < PARTS - 1; ++i) {
                double partValue = std::floor(value / std::pow(2.0, STRIDE_BITS * (PARTS - i - 2)));
                lows[i] = static_cast<uint_t>(partValue);
                value -= partValue * std::pow(2.0, STRIDE_BITS * (PARTS - i - 2));
            }
        }

        return *this;
    }

    constexpr inline spc_coord& from_int64(int64_t value){
        if (value < 0){ high -= 1; }
        low[PARTS - 2] = static_cast<uint_t>(value);
        return *this;
    }

    constexpr operator double() const {
        return to_double();
    }

    constexpr spc_coord& operator=(double value) {
        return from_double(value);
    }

    constexpr inline spc_coord operator+(const spc_coord& rhs) const {
        spc_coord result;
        bool carry = false;
        for (int i = N - 1; i >= 0; --i) {
            result.lows[i] = lows[i] + rhs.lows[i] + (carry ? 1 : 0);
            carry = result.lows[i] < lows[i];
        }
        result.high = high + rhs.high + (carry ? 1 : 0);
        return result;
    }

    constexpr inline spc_coord operator-(const spc_coord& rhs) const {
        spc_coord result;
        bool borrow = false;
        for (int i = PARTS - 1; i >= 0; --i) {
            result.lows[i] = lows[i] - rhs.lows[i] - (borrow ? 1 : 0);
            borrow = result.lows[i] > lows[i];
        }
        result.high = high - rhs.high - (borrow ? 1 : 0);
        return result;
    }

    constexpr inline spc_coord operator*(const spc_coord& rhs) const {
        spc_coord result;
        double lhs_double = static_cast<double>(*this);
        double rhs_double = static_cast<double>(rhs);
        double result_double = lhs_double * rhs_double;
        result = result_double;
        return result;
    }

    constexpr inline spc_coord operator/(const spc_coord& rhs) const {
        spc_coord result;
        double lhs_double = static_cast<double>(*this);
        double rhs_double = static_cast<double>(rhs);
        double result_double = lhs_double / rhs_double;
        result = result_double;
        return result;
    }

    constexpr inline spc_coord operator+(double rhs) const {
        return (*this) + spc_coord().from_double(rhs);
    }

    constexpr inline spc_coord operator-(double rhs) const {
        return (*this) - spc_coord().from_double(rhs);
    }

    constexpr inline spc_coord operator+(int64_t rhs) const {
        return (*this) + spc_coord().from_int64(rhs);
    }

    constexpr inline spc_coord operator-(int64_t rhs) const {
        return (*this) - spc_coord().from_int64(rhs);
    }


    friend std::ostream& operator<<(std::ostream& os, const spc_coord& num) {
        os << num.high;
        for (int i = 0; i < PARTS - 1; ++i) {
            os << "." << num.lows[i];
        }
        return os;
    }
};

typedef spc_coord<2> spc_coord_128_64_t;
typedef spc_coord<3> spc_coord_192_64_t;
typedef spc_coord<4> spc_coord_256_64_t;
typedef spc_coord<8> spc_coord_512_64_t;
typedef spc_coord<16> spc_coord_1024_64_t;
