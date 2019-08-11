#ifndef __SPYC_UTILS_HH__
#define __SPYC_UTILS_HH__

#include <boost/iterator/filter_iterator.hpp>

#include <functional>
#include <iterator>

namespace spyc::utils {

    template <class Container>
    class FilteredRange {
        using element = typename Container::value_type;
        using predicate = std::function<bool(const element&)>;
        using c_iter = typename Container::const_iterator;
        using f_iter = boost::filter_iterator<predicate, c_iter>;

    public:
        FilteredRange() = delete;
        FilteredRange(const Container& c, predicate filter)
            : container{c}, f(filter), c_begin{std::cbegin(c)},
            c_end{std::cend(c)} {}

        f_iter begin() const {
            return boost::make_filter_iterator<predicate>(f, c_begin);
        }

        f_iter end() const {
            return boost::make_filter_iterator<predicate>(f, c_end);
        }

    private:
        const Container& container;
        predicate f;
        c_iter c_begin;
        c_iter c_end;
    };

    template <typename T>
    FilteredRange<T> make_filtered_range(const T& t, std::function<bool(const typename T::value_type&)> f)
    {
        return FilteredRange<T>(t, f);
    }

}

#endif /* __SPYC_UTILS_HH__ */
