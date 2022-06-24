#ifndef GL_ALGO_H
#define GL_ALGO_H

#include <vector>

namespace gl {
    template<typename T, typename Cmp>
    void VectorUnique(std::vector<T>& v, Cmp cmp) {
        std::sort(v.begin(), v.end(), cmp);
        v.erase(std::unique(v.begin(), v.end()), v.end());
    }

    template<typename T>
    void VectorUnique(std::vector<T>& v) {
        std::sort(v.begin(), v.end(), cmp);
        v.erase(std::unique(v.begin(), v.end()), v.end());
    }

    template<typename T>
    bool VectorErase(std::vector<T>& v, const T& t) {
        auto it = std::find(v.begin(), v.end(), t);
        if (it == v.end())
            return false;
        v.erase(it);
        return true;
    }

    template<typename T>
    int CountVectorIntersection(std::vector<T>& v1, std::vector<T>& v2) {
        int ret = 0;
        std::sort(v1.begin(), v1.end());
        std::sort(v2.begin(), v2.end());
        for (int i1 = 0, i2 = 0; i1 < v1.size() && i2 < v2.size();) {
            if (v1[i1] == v2[i2])
                ++i1, ++i2, ++ret;
            else if (v1[i1] < v2[i2])
                ++i1;
            else
                ++i2;
        }
        return ret;
    }
    
    //if sort ascendly, greater_or_less shouble be false. Else true
    template<typename T,typename Cmp>
    int CountVectorIntersection(std::vector<T>& v1, std::vector<T>& v2, Cmp cmp, bool greater_or_less) {
        int ret = 0;
        std::sort(v1.begin(), v1.end(), cmp);
        std::sort(v2.begin(), v2.end(), cmp);
        for (int i1 = 0, i2 = 0; i1 < v1.size() && i2 < v2.size();) {
            if (v1[i1] == v2[i2])
                ++i1, ++i2, ++ret;
            else if (cmp(v1, v2) ^ greater_or_less)
                ++i1;
            else
                ++i2;
        }
        return ret;
    }

    template<typename T>
    void VectorIntersection(std::vector<T>& v1, std::vector<T>& v2, std::vector<T>& ret) {
        std::sort(v1.begin(), v1.end());
        std::sort(v2.begin(), v2.end());
        std::set_intersection(v1.begin(), v1.end(), v2.begin(), v2.end(), std::back_inserter(res));
    }

    template<typename T, typename Cmp>
    void VectorIntersection(std::vector<T>& v1, std::vector<T>& v2, std::vector<T>& ret, Cmp cmp) {
        std::sort(v1.begin(), v1.end(), cmp);
        std::sort(v2.begin(), v2.end(), cmp);
        std::set_intersection(v1.begin(), v1.end(), v2.begin(), v2.end(), std::back_inserter(res));
    }

    template<typename T>
    void VectorUnion(std::vector<T>& v1, std::vector<T>& v2, std::vector<T>& ret) {
        std::sort(v1.begin(), v1.end());
        std::sort(v2.begin(), v2.end());
        std::set_union(v1.begin(), v1.end(), v2.begin(), v2.end(), std::back_inserter(res));
    }

    template<typename T, typename Cmp>
    void VectorUnion(std::vector<T>& v1, std::vector<T>& v2, std::vector<T>& ret, Cmp cmp) {
        std::sort(v1.begin(), v1.end(), cmp);
        std::sort(v2.begin(), v2.end(), cmp);
        std::set_union(v1.begin(), v1.end(), v2.begin(), v2.end(), std::back_inserter(res));
    }
}

#endif