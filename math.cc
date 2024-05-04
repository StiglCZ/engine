#include "math.hh"

// Is inf or not a number
inline int ft_isnan(fx x){
    return (x != x);
}

inline int ft_isinf(fx x){
    if (x == (-1.0 / 0.0))
        return ((!ft_isnan(x) && ft_isnan(x - x)) * -1);
    return (!ft_isnan(x) && ft_isnan(x - x));
}

[[deprecated("Use <cmath>")]]
fx ft_fmod(fx x, fx y){
    if (ft_isinf(x)) return (-0.0 / 0.0);
    if (ft_isinf(y)) return (x);
    return ((x) - ((long)(x / y)) * (y));
}

[[deprecated("Use <cmath>")]]
fx cos(fx x){
    fx result,term;
    unsigned int epsilon;
    if (ft_isinf(x))return (0.0 / 0.0);
    result = 1.0;
    term = 1.0;
    epsilon = 0;
    x = ft_fmod(x, 2.0 * M_PI);
    while (++epsilon, fabs(term) > 1E-15){
        term = -term * x * x / (2.0 * epsilon - 1.0) / (2.0 * epsilon);
        result += term;
    }
    return (result);
}

[[deprecated("Use <cmath>")]]
fx sin(fx x){
    fx result, term;
    unsigned int epsilon;
    if (ft_isinf(x)) return (0.0 / 0.0);
    x = ft_fmod(x, 2.0 * M_PI);
    epsilon = 3;
    term = x;
    result = x;
    while (fabs(term) > 1E-15){
        term = -term * x * x / (epsilon * (epsilon - 1.0));
        result += term;
        epsilon += 2;
    }
    return (result);
}

[[deprecated("Use <cmath>")]]
fx tan(fx source) { return (sin(source) / cos(source)); }
