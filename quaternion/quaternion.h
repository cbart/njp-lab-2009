/** Quaternion.                                            *
 * author: Cezary Bartoszuk <cbart@students.mimuw.edu.pl>  *
 *     id: cb277617@students.mimuw.edu.pl                  */

#ifndef _QUATERNION_H_
#define _QUATERNION_H_

#include <iostream>
#include <stdexcept>
#include "debug_tools.h"
#include "safe_bool.h"

typedef double Real;
typedef unsigned long UInteger;

const Real RZERO = 0.0;

const Real RONE = 1.0;

class DivideByZeroException : public std::runtime_error {

    public:
        /** Creates new DivideByZeroException. */
        DivideByZeroException();

};

/* TODO:
 *   * use boost abstract templates to check if we have all needed operators.
 */

class QuaternionSequence;

/* `QContainer` interface. */
class IQContainer {

    public:
        /* <Pure virtual> copy constructor. */
        virtual IQContainer* clone() const = 0;
        /* Abstract getters. */
        virtual Real getA() const = 0;
        virtual Real getB() const = 0;
        virtual Real getC() const = 0;
        virtual Real getD() const = 0;
        /* Abstract setters. */
        virtual void setA(Real a) = 0;
        virtual void setB(Real b) = 0;
        virtual void setC(Real c) = 0;
        virtual void setD(Real d) = 0;

};

class Quaternion : public SafeBool<> {
    /** Signature of a class implementing quaternions. */

    public:

        friend class ::QuaternionSequence;

    private:

        /* Creates `Quaternion` with given `IQContainer`. */
        explicit Quaternion(IQContainer* cnt);

    protected:

        class QContainer : public IQContainer {
            /** Contains quaternion elements. */

            private:

                Real a_val;
                Real b_val;
                Real c_val;
                Real d_val;

            public:

                QContainer();
                QContainer(Real a, Real b, Real c, Real d);
                QContainer(const IQContainer& cnt);
                virtual ~QContainer();
                QContainer& operator=(const QContainer& cnt);
                QContainer* clone() const;  /* @Override */
                Real getA() const;          /* @Override */
                Real getB() const;          /* @Override */
                Real getC() const;          /* @Override */
                Real getD() const;          /* @Override */
                void setA(Real a);          /* @Override */
                void setB(Real b);          /* @Override */
                void setC(Real c);          /* @Override */
                void setD(Real d);          /* @Override */

        };

        IQContainer* val;

        /* Bool testing.
         * `self` is `false` if and only if `self` == (0, 0, 0, 0).
         * `self` is `true` in all other cases. */
        bool booleanTest() const; /* @Override; */

    public:

        /* Creates `zero` quaternion (0, 0, 0, 0). */
        Quaternion();

        /* Creates real quaternion (a, 0, 0, 0). */
        Quaternion(Real a);

        /* Creates quaternion which behaves like complex number
         * with Re = `a` and Im = `b`: (a, b, 0, 0). */
        Quaternion(Real a, Real b);

        /* Creates common quaternion (a, b, c, d). */
        Quaternion(Real a, Real b, Real c, Real d);

        /* Creates copy of quaternion `q`. */
        Quaternion(const Quaternion& q);

        /* Destructor. */
        ~Quaternion();

        /* Returns quaternion's first coordinate. */
        Real r() const;

        /* Returns quaternion's second coordinate. */
        Real i() const;

        /* Returns quaternion's third coordinate. */
        Real j() const;

        /* Returns quaternion's fourth coordinate. */
        Real k() const;

        /* Assigns given quaternion to `self`. */
        Quaternion& operator=(const Quaternion& q);

        /* Unary minus (prefix):
         * (a, b, c, d) -> (-a, -b, -c, -d). */
        Quaternion operator-() const;

        /* Add and assign:
         * self(a, b, c, d) += q(a, b, c, d) ->
         * self(self.a + q.a, self.b + q.b, self.c + q.c, self.d + q.d). */
        Quaternion& operator+=(const Quaternion& q);

        /* Subtract and assign:
         * self(a, b, c, d) -= q(a, b, c, d) ->
         * self(self.a - q.a, self.b - q.b, self.c - q.c, self.d - q.d). */
        Quaternion& operator-=(const Quaternion& q);

        /* Multiply and assign:
         * self(a, b, c, d) *= q(a, b, c, d) ->
         * self(self.a * q.a - self.b * q.b - self.c * q.c - self.d * q.d,
         *      self.a * q.b + self.b * q.a + self.c * q.d - self.d * q.c,
         *      self.a * q.c - self.b * q.d + self.c * q.a * self.d * q.b,
         *      self.a * q.d + self.b * q.c - self.c * q.b + self.d * q.a) */
        Quaternion& operator*=(const Quaternion& q);

        /* Division and assign:
         * self(a, b, c, d) /= q(a, b, c, d) ->
         * self((self * conj(q)) / (||q|| ^ 2)) */
        Quaternion& operator/=(const Quaternion& q)
            throw (DivideByZeroException);

        /* Add real and assign:
         * self(a, b, c, d) += r -> self(a + r, b, c, d); */
        Quaternion& operator+=(const Real& r);

        /* Subtract real and assign:
         * self(a, b, c, d) -= r -> self(a - r, b, c, d); */
        Quaternion& operator-=(const Real& r);

        /* Multiply by real and assign:
         * self(a, b, c, d) *= r -> self(r * a, r * b, r * c, r * d); */
        Quaternion& operator*=(const Real& r);

        /* Divide by real and assign:
         * self(a, b, c, d) /= r -> self(a / r, b / r, c / r, d / r); */
        Quaternion& operator/=(const Real& r)
            throw (DivideByZeroException);

        /* Add:
         * self(a, b, c, d) + q(a, b, c, d) ->
         * (self.a + q.a, self.b + q.b, self.c + q.c, self.d + q.d). */
        friend const Quaternion operator+
            (const Quaternion& p, const Quaternion& q);

       /* Subtract:
         * self(a, b, c, d) - q(a, b, c, d) ->
         * (self.a - q.a, self.b - q.b, self.c - q.c, self.d - q.d). */
        friend const Quaternion operator-
            (const Quaternion& p, const Quaternion& q);

        /* Multiply (hamilton product):
         * self(a, b, c, d) * q(a, b, c, d) ->
         * (self.a * q.a - self.b * q.b - self.c * q.c - self.d * q.d,
         *  self.a * q.b + self.b * q.a + self.c * q.d - self.d * q.c,
         *  self.a * q.c - self.b * q.d + self.c * q.a + self.d * q.b,
         *  self.a * q.d + self.b * q.c - self.c * q.b + self.d * q.a) */
        friend const Quaternion operator*
            (const Quaternion& p, const Quaternion& q);

        /* Division:
         * self(a, b, c, d) / q(a, b, c, d) ->
         * (self * conj(q)) / (||q|| ^ 2) */
        friend const Quaternion operator/
            (const Quaternion& p, const Quaternion& q)
          throw (DivideByZeroException);

        /* Add real to quaternion:
         * self(a, b, c, d) + r -> (a + r, b, c, d); */
        friend const Quaternion operator+(const Quaternion& q, const Real& r);
        friend const Quaternion operator+(const Real& r, const Quaternion& q);

        /* Subtract real from quaternion:
         * self(a, b, c, d) - r -> (a - r, b, c, d); */
        friend const Quaternion operator-(const Quaternion& q, const Real& r);
        friend const Quaternion operator-(const Real& r, const Quaternion& q);

        /* Multiply quaternion by real:
         * self(a, b, c, d) * r -> (r * a, r * b, r * c, r * d); */
        friend const Quaternion operator*(const Quaternion& q, const Real& r);
        friend const Quaternion operator*(const Real& r, const Quaternion& q);

        /* Divide by real and assign:
         * self(a, b, c, d) /= r -> self(a / r, b / r, c / r, d / r); */
        friend const Quaternion operator/(const Quaternion& q, const Real& r)
            throw (DivideByZeroException);
        friend const Quaternion operator/(const Real& r, const Quaternion& q)
            throw (DivideByZeroException);

        /* Are same?
         * self(a, b, c, d) == q(a, b, c, d) if and only if
         * self.a == q.a AND
         * self.b == q.b AND
         * self.c == q.c AND
         * self.d == q.d. */
        friend bool operator==(const Quaternion& p, const Quaternion& q);

        /* Are not same?
         * self(a, b, c, d) != q(a, b, c, d) if and only if
         * self.a != q.a OR
         * self.b != q.b OR
         * self.c != q.c OR
         * self.d != q.d. */
        friend bool operator!=(const Quaternion& p, const Quaternion& q);

        /* Pushing quaternion `q` to output stream `os`. */
        friend std::ostream& operator<<(std::ostream& os, const Quaternion& q);

};

/* Returns real (scalar part) of `q` quaternion; that means:
 * (a, _, _, _) -> (a, 0, 0, 0). */
const Quaternion re(const Quaternion& q);

/* Returns pure imaginary (vector part) of `q` quaternion; that means:
 * (_, b, c, d) -> (0, b, c, d). */
const Quaternion im(const Quaternion& q);

/* Returns conjugated quaternion `q`:
 * (a, b, c, d) -> (a, -b, -c, -d). */
const Quaternion conj(const Quaternion& q);

/* Returns unit of quaternion `q`:
 * q(a, b, c, d) -> (a / ||q||, b / ||q||, c / ||q||, d / ||q||)
 * where ||.|| is `norm` declared below. */
const Quaternion unit(const Quaternion& q);

/* Returns determinant of given quaternion:
 * (a, b, c, d) -> a ^ 2 + b ^ 2 + c ^ 2 + d ^ 2. */
const Real det(const Quaternion& q);

/* Returns norm of quaternion `q`:
 * (a, b, c, d) -> (a ^ 2 + b ^ 2 + c ^ 2 + d ^ 2) ^ 0.5. */
const Real norm(const Quaternion& q);

/* Returns reciprocal of a quaternion `q`:
 * q -> 1/q, that is:
 * q(a, b, c, d) -> (a, -b, -c, -d) / ||q||. */
const Quaternion reciprocal(const Quaternion& q)
    throw (DivideByZeroException);

/* Returns dot product of given imaginary quaternions:
 * (0, i1, j1, k1) . (0, i2, j2, k2) -> i1 * i2 + j1 * j2 + k1 * k2. */
const Real dotProd(const Quaternion& p, const Quaternion& q);

/* Returns cross product of given imaginary quaternions:
 * (0, b1, c1, d1) x (0, b2, c2, d2) ->
 * (0, c1 * d2 - d1 * c2, d1 * b2 - b1 * d2, b1 * c2 - c1 * b2) */
const Quaternion crossProd(const Quaternion& p, const Quaternion& q);

/* Returns exponent of given quaternion (q = (a, b, c, d)):
 * exp(q) = (e ^ a) * (cos||v|| + Uv * sin||v||),
 * where `v` is pure imaginary of `q` (v = im(q)),
 * and `Uv` is unit of `v` (Uv = unit(v)). */
const Quaternion exp(const Quaternion& q);

/* Returns natural logarithm of given quaternion (q = (a, b, c, d)):
 * ln(q) = ln||q|| + Uv * arccos(a / ||q||),
 * where `v` is pure imaginary of `q` (v = im(q)),
 * and `Uv` is unit of `v` (Uv = unit(v)). */
const Quaternion log(const Quaternion& q);

/* Natural power of a quaternion. */
const Quaternion pow(const Quaternion& q, UInteger i);

/* Returns `i` quaternion (0., 1., 0., 0.). */
const Quaternion I();

/* Returns `j` quaternion (0., 0., 1., 0.). */
const Quaternion J();

/* Returns `k` quaternion (0., 0., 0., 1.). */
const Quaternion K();

#endif
