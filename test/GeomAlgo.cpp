#include "GeomAlgo.h"
#include "Rational.h"
#include "Predicate.h"

namespace gl {
    bool IsDegenerate(const Vector3& v0, const Vector3& v1, const Vector3& v2, const Vector3& v3) {
        if (Predicates::Orient3d(v0, v1, v2, v3) == Predicates::ORI_ZERO)
            return true;
        return false;
    }

    double AMIPSenergyAux(const std::array<double, 12>& T) {
        double helper_0[12];
        helper_0[0] = T[0];
        helper_0[1] = T[1];
        helper_0[2] = T[2];
        helper_0[3] = T[3];
        helper_0[4] = T[4];
        helper_0[5] = T[5];
        helper_0[6] = T[6];
        helper_0[7] = T[7];
        helper_0[8] = T[8];
        helper_0[9] = T[9];
        helper_0[10] = T[10];
        helper_0[11] = T[11];
        double helper_1 = helper_0[2];
        double helper_2 = helper_0[11];
        double helper_3 = helper_0[0];
        double helper_4 = helper_0[3];
        double helper_5 = helper_0[9];
        double helper_6 = 0.577350269189626 * helper_3 - 1.15470053837925 * helper_4 + 0.577350269189626 * helper_5;
        double helper_7 = helper_0[1];
        double helper_8 = helper_0[4];
        double helper_9 = helper_0[7];
        double helper_10 = helper_0[10];
        double helper_11 = 0.408248290463863 * helper_10 + 0.408248290463863 * helper_7 + 0.408248290463863 * helper_8 -
            1.22474487139159 * helper_9;
        double helper_12 = 0.577350269189626 * helper_10 + 0.577350269189626 * helper_7 - 1.15470053837925 * helper_8;
        double helper_13 = helper_0[6];
        double helper_14 = -1.22474487139159 * helper_13 + 0.408248290463863 * helper_3 + 0.408248290463863 * helper_4 +
            0.408248290463863 * helper_5;
        double helper_15 = helper_0[5];
        double helper_16 = helper_0[8];
        double helper_17 = 0.408248290463863 * helper_1 + 0.408248290463863 * helper_15 - 1.22474487139159 * helper_16 +
            0.408248290463863 * helper_2;
        double helper_18 = 0.577350269189626 * helper_1 - 1.15470053837925 * helper_15 + 0.577350269189626 * helper_2;
        double helper_19 = 0.5 * helper_13 + 0.5 * helper_4;
        double helper_20 = 0.5 * helper_8 + 0.5 * helper_9;
        double helper_21 = 0.5 * helper_15 + 0.5 * helper_16;
        double helper_22 = (helper_1 - helper_2) * (helper_11 * helper_6 - helper_12 * helper_14) -
            (-helper_10 + helper_7) * (-helper_14 * helper_18 + helper_17 * helper_6) +
            (helper_3 - helper_5) * (-helper_11 * helper_18 + helper_12 * helper_17);
        double res = -(helper_1 * (-1.5 * helper_1 + 0.5 * helper_2 + helper_21) +
            helper_10 * (-1.5 * helper_10 + helper_20 + 0.5 * helper_7) +
            helper_13 * (-1.5 * helper_13 + 0.5 * helper_3 + 0.5 * helper_4 + 0.5 * helper_5) +
            helper_15 * (0.5 * helper_1 - 1.5 * helper_15 + 0.5 * helper_16 + 0.5 * helper_2) +
            helper_16 * (0.5 * helper_1 + 0.5 * helper_15 - 1.5 * helper_16 + 0.5 * helper_2) +
            helper_2 * (0.5 * helper_1 - 1.5 * helper_2 + helper_21) +
            helper_3 * (helper_19 - 1.5 * helper_3 + 0.5 * helper_5) +
            helper_4 * (0.5 * helper_13 + 0.5 * helper_3 - 1.5 * helper_4 + 0.5 * helper_5) +
            helper_5 * (helper_19 + 0.5 * helper_3 - 1.5 * helper_5) +
            helper_7 * (0.5 * helper_10 + helper_20 - 1.5 * helper_7) +
            helper_8 * (0.5 * helper_10 + 0.5 * helper_7 - 1.5 * helper_8 + 0.5 * helper_9) +
            helper_9 * (0.5 * helper_10 + 0.5 * helper_7 + 0.5 * helper_8 - 1.5 * helper_9))
            / std::cbrt(helper_22 * helper_22);
        //                 * pow(pow((helper_1 - helper_2) * (helper_11 * helper_6 - helper_12 * helper_14) -
        //                         (-helper_10 + helper_7) * (-helper_14 * helper_18 + helper_17 * helper_6) +
        //                         (helper_3 - helper_5) * (-helper_11 * helper_18 + helper_12 * helper_17), 2),
        //                     -0.333333333333333);
        return res;
    }

    double AMIPSenergy(const std::array<double, 12>& T) {
        double res = AMIPSenergyAux(T);
        //if (use_old_energy) {
        //    return res;
        //}

        if (res > 1e8) {
            //        //fortest
            //        if (res > 1e10) {
            //            cout << std::setprecision(16) << res << endl;
            //            for (int i = 0; i < T.size(); i++) {
            //                if (i % 3 == 0)
            //                    cout << endl;
            //                cout << T[i] << ", ";
            //            }
            //            cout << endl;
            //            char c;
            //            cin >> c;
            //        }
            //        //fortest

            //        //fortest
            //        cnt_large++;
            //        if(!is_energy_unstable(T, res)){
            //            cout<<(cnt_stable++)<<"/"<<cnt_large<<endl;
            //        }
            //        //fortest

            if (IsDegenerate(Vector3(T[0], T[1], T[2]), Vector3(T[3], T[4], T[5]), Vector3(T[6], T[7], T[8]),
                Vector3(T[9], T[10], T[11]))) {
                throw("energy computation degenerate found!!!");
                return std::numeric_limits<double>::infinity();
            }

            std::array<triwild::Rational, 12> r_T;
            for (int j = 0; j < 12; j++)
                r_T[j] = T[j];
            const triwild::Rational twothird = triwild::Rational(2) / triwild::Rational(3);
            triwild::Rational tmp = ((-r_T[1 + 2] + r_T[1 + 5]) * r_T[1 + 1] + r_T[1 + 2] * r_T[1 + 7] +
                (r_T[1 + -1] - r_T[1 + 5]) * r_T[1 + 4] - r_T[1 + -1] * r_T[1 + 7]) * r_T[1 + 9] +
                ((r_T[1 + 2] - r_T[1 + 5]) * r_T[1 + 0] - r_T[1 + 2] * r_T[1 + 6] +
                    (-r_T[1 + -1] + r_T[1 + 5]) * r_T[1 + 3] + r_T[1 + -1] * r_T[1 + 6]) * r_T[1 + 10] +
                (-r_T[1 + 2] * r_T[1 + 7] + (-r_T[1 + 8] + r_T[1 + 5]) * r_T[1 + 4] +
                    r_T[1 + 8] * r_T[1 + 7]) * r_T[1 + 0] +
                (r_T[1 + 2] * r_T[1 + 6] + (r_T[1 + 8] - r_T[1 + 5]) * r_T[1 + 3] - r_T[1 + 8] * r_T[1 + 6]) *
                r_T[1 + 1] + (r_T[1 + 3] * r_T[1 + 7] - r_T[1 + 4] * r_T[1 + 6]) * (r_T[1 + -1] - r_T[1 + 8]);
            if (tmp == 0)
                return std::numeric_limits<double>::infinity();

            auto res_r = triwild::Rational(27) / 16 *
                pow(tmp, -2) * pow(r_T[1 + 9] * r_T[1 + 9] +
                    (-twothird * r_T[1 + 0] - twothird * r_T[1 + 3] - twothird * r_T[1 + 6]) *
                    r_T[1 + 9] + r_T[1 + 10] * r_T[1 + 10] +
                    (-twothird * r_T[1 + 1] - twothird * r_T[1 + 4] - twothird * r_T[1 + 7]) *
                    r_T[1 + 10] + r_T[1 + 0] * r_T[1 + 0] +
                    (-twothird * r_T[1 + 3] - twothird * r_T[1 + 6]) * r_T[1 + 0] +
                    r_T[1 + 1] * r_T[1 + 1] +
                    (-twothird * r_T[1 + 4] - twothird * r_T[1 + 7]) * r_T[1 + 1] +
                    r_T[1 + 2] * r_T[1 + 2] +
                    (-twothird * r_T[1 + -1] - twothird * r_T[1 + 8] - twothird * r_T[1 + 5]) *
                    r_T[1 + 2] + r_T[1 + 3] * r_T[1 + 3] - twothird * r_T[1 + 3] * r_T[1 + 6] +
                    r_T[1 + 4] * r_T[1 + 4] - twothird * r_T[1 + 4] * r_T[1 + 7] +
                    r_T[1 + 5] * r_T[1 + 5] +
                    (-twothird * r_T[1 + -1] - twothird * r_T[1 + 8]) * r_T[1 + 5] -
                    twothird * r_T[1 + -1] * r_T[1 + 8] + r_T[1 + -1] * r_T[1 + -1] +
                    r_T[1 + 8] * r_T[1 + 8] + r_T[1 + 6] * r_T[1 + 6] + r_T[1 + 7] * r_T[1 + 7], 3);
            return std::cbrt(res_r.to_double());
        }
        else {
            return res;
        }
    }

    void FastWindingNumber(const Eigen::MatrixXd& v, const Eigen::MatrixXi& f, const Eigen::MatrixXd& p,
        Eigen::VectorXd& w) {
        igl::winding_number(v, f, p, w);
    }
}

