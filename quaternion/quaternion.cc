/** author: Cezary Bartoszuk <cbart@students.mimuw.edu.pl> *
 *    user: cb277617@students.mimuw.edu.pl                 */

#include <cmath>
#include <iostream>
#include <stdexcept>
#include "./quaternion.h"

/** DEBUGLEVEL:                      *
 *  0 - no debug;                    *
 *  1 - assertions ON;               *
 *  2 - 1 and messages for errors;   *
 *  3 - 2 and messages for warnings; *
 *  4 - 3 and messages for info;     *
 *  5 - paranoid level;              */

#ifdef DEBUGLEVEL
    const int DEBUG_LEVEL = DEBUGLEVEL;
#else
    const int DEBUG_LEVEL = 0;
#endif

/* Exception raised when division by zero is encountered. */
DivideByZeroException::DivideByZeroException()
  : std::runtime_error("(EX) Attempted to divide by zero!\n")
{
    if(DEBUG_LEVEL > 1)
        std::cerr << "(EX) DivideByZero::DivideByZero() thrown!\n"
                  << std::flush;
}

/* Inner `Quaternion`s value container constructor. */
Quaternion::QContainer::QContainer()
  : a_val(0.),
    b_val(0.),
    c_val(0.),
    d_val(0.)
{
    if(DEBUG_LEVEL > 3)
        std::cerr << "(II) "
                     "Quaternion::QContainer::QContainer();\n"
                  << std::flush;
}

/* Inner `Quaternion`s value container constructor. */
Quaternion::QContainer::QContainer(Real a, Real b, Real c, Real d)
  : a_val(a),
    b_val(b),
    c_val(c),
    d_val(d)
{
    if(DEBUG_LEVEL > 3)
        std::cerr << "(II) "
                     "Quaternion::QContainer::QContainer"
                     "(Real a, Real b, Real c, Real d);\n"
                  << std::flush;
}

/* Inner `Quaternion`s value container copy constructor. */
Quaternion::QContainer::QContainer(const IQContainer& cnt)
  : a_val(cnt.getA()),
    b_val(cnt.getB()),
    c_val(cnt.getC()),
    d_val(cnt.getD())
{
    if(DEBUG_LEVEL > 3)
        std::cerr << "(II) "
                     "Quaternion::QContainer::QContainer"
                     "(const IQContainer& cnt);\n"
                  << std::flush;
}

/* `QContainer`s destructor. */
Quaternion::QContainer::~QContainer()
{
    if(DEBUG_LEVEL > 3)
        std::cerr << "(II) Quaternion::QContainer::~QContainer();\n"
                  << std::flush;
}

Quaternion::QContainer& Quaternion::QContainer::operator=(const QContainer& cnt)
{
    if(DEBUG_LEVEL > 4)
        std::cerr << "(II) "
                     "Quaterinon::QContainer::operator="
                     "(const QContainer& cnt);\n"
                  << std::flush;
    a_val = cnt.a_val;
    b_val = cnt.b_val;
    c_val = cnt.c_val;
    d_val = cnt.d_val;
    return *this;
}

Quaternion::QContainer* Quaternion::QContainer::clone() const
{
    if(DEBUG_LEVEL > 3)
        std::cerr << "(II) "
                     "Quaternion::QContainer* "
                     "Quaternion::QContainer::clone();\n"
                  << std::flush;
    return new QContainer(*this);
}

/* `QContainer` getter. */
Real Quaternion::QContainer::getA() const
{
    return a_val;
}

/* `QContainer` getter. */
Real Quaternion::QContainer::getB() const
{
    return b_val;
}

/* `QContainer` getter. */
Real Quaternion::QContainer::getC() const
{
    return c_val;
}

/* `QContainer` getter. */
Real Quaternion::QContainer::getD() const
{
    return d_val;
}

/* `QContainer` setter. */
void Quaternion::QContainer::setA(Real a)
{
    a_val = a;
}

/* `QContainer` setter. */
void Quaternion::QContainer::setB(Real b)
{
    b_val = b;
}

/* `QContainer` setter. */
void Quaternion::QContainer::setC(Real c)
{
    c_val = c;
}

/* `QContainer` setter. */
void Quaternion::QContainer::setD(Real d)
{
    d_val = d;
}

/* Creates `Quaternion` with given `QContainer`. */
Quaternion::Quaternion(IQContainer* cnt)
    : val(cnt)
{
    if(DEBUG_LEVEL > 3)
        std::cerr << "(II) "
                     "Quaternion::Quaternion"
                     "(IQContainer* cnt);\n"
                  << std::flush;
}

/* Creates `zero` quaternion (0, 0, 0, 0). */
Quaternion::Quaternion()
    : val(new Quaternion::QContainer(0., 0., 0., 0.))
{
    if(DEBUG_LEVEL > 3)
        std::cerr << "(II) Quaternion::Quaternion();\n" << std::flush;
}

/* Creates real quaternion (a, 0, 0, 0). */
Quaternion::Quaternion(Real a)
  : val(new Quaternion::QContainer(a, 0., 0., 0.))
{
    if(DEBUG_LEVEL > 3)
        std::cerr << "(II) Quaternion::Quaternion(" << a << ");\n"
                  << std::flush;
}

/* Creates quaternion which behaves like complex number
 * with Re = `a` and Im = `b`: (a, b, 0, 0). */
Quaternion::Quaternion(Real a, Real b)
  : val(new Quaternion::QContainer(a, b, 0., 0.))
{
    if(DEBUG_LEVEL > 3)
        std::cerr << "(II) Quaternion::Quaternion("
                  << a << ", " << b << ");\n" << std::flush;
}

/* Creates common quaternion (a, b, c, d). */
Quaternion::Quaternion(Real a, Real b, Real c, Real d)
  : val(new Quaternion::QContainer(a, b, c, d))
{
    if(DEBUG_LEVEL > 3)
        std::cerr << "(II) Quaternion::Quaternion("
                  << a << ", " << b << ", "
                  << c << ", " << d << ");\n" << std::flush;
}

/* Creates copy of quaternion `q`. */
Quaternion::Quaternion(const Quaternion& q)
  : val(q.val->clone())
{
    if(DEBUG_LEVEL > 3)
        std::cerr << "(II) Quaternion::Quaternion(const Quaternion&);\n"
                     "\tCalled with: " << q << ".\n" << std::flush;
}

/* Destructor. */
Quaternion::~Quaternion()
{
    if(DEBUG_LEVEL > 3)
        std::cerr << "(II) Quaternion::~Quaternion();\n" << std::flush;
    delete val;
}

/* Returns quaternion's first coordinate. */
Real Quaternion::r() const
{
    if(DEBUG_LEVEL > 4)
        std::cerr << "(II) Quaternion::r();\n"
                     "\tReturns " << val->getA() << ".\n" << std::flush;
    return val->getA();
}

/* Returns quaternion's second coordinate. */
Real Quaternion::i() const {
    if(DEBUG_LEVEL > 4)
        std::cerr << "(II) Quaternion::i();\n"
                     "\tReturns " << val->getB() << ".\n" << std::flush;
    return val->getB();
}

/* Returns quaternion's third coordinate. */
Real Quaternion::j() const {
    if(DEBUG_LEVEL > 4)
        std::cerr << "(II) Quaternion::j();\n"
                     "\tReturns " << val->getC() << ".\n" << std::flush;
    return val->getC();
}

/* Returns quaternion's fourth coordinate. */
Real Quaternion::k() const
{
    if(DEBUG_LEVEL > 4)
        std::cerr << "(II) Quaternion::k();\n"
                     "\tReturns " << val->getD() << ".\n" << std::flush;
    return val->getD();
}

/* Assigns given quaternion to `self`. */
Quaternion& Quaternion::operator=(const Quaternion& q)
{
    if(DEBUG_LEVEL > 4)
        std::cerr << "(II) Quaternion::operator=(const Quaternion&);\n"
                     "\tLeft:  " << *this << ";\n"
                     "\tRight: " << q << ".\n" << std::flush;
    if(this == &q) {
        if(DEBUG_LEVEL == 3)
            std::cerr << "(II) Quaternion::operator=(const Quaternion&);\n"
                         "\tLeft:  " << *this << ";\n"
                         "\tRight: " << q << ".\n";
        if(DEBUG_LEVEL > 2)
            std::cerr << "(WW)\tRequested self-assignment.\n" << std::flush;
    }
    else { /* this != &q: */
        delete val;
        val = q.val->clone();
    }

    return *this;
}

/* Unary minus (prefix):
 * (a, b, c, d) -> (-a, -b, -c, -d). */
Quaternion Quaternion::operator-() const
{
    if(DEBUG_LEVEL > 4)
        std::cerr << "(II) Quaternion::operator-() const;\n" << std::flush;
    return Quaternion(-val->getA(), -val->getB(), -val->getC(), -val->getD());
}

/* Add and assign:
 * self(a, b, c, d) += q(a, b, c, d) ->
 * self(self.a + q.a, self.b + q.b, self.c + q.c, self.d + q.d). */
Quaternion& Quaternion::operator+=(const Quaternion& q)
{
    if(DEBUG_LEVEL > 4)
        std::cerr << "(II) Quaternion::operator+=(const Quaternion& q);\n"
                     "\tCalled with: " << q << ".\n"
                     "\tSelf: " << *this << " (before assignment).\n";
    val->setA(val->getA() + q.val->getA());
    val->setB(val->getB() + q.val->getB());
    val->setC(val->getC() + q.val->getC());
    val->setD(val->getD() + q.val->getD());
    if(DEBUG_LEVEL > 4)
        std::cerr << "\tSelf: " << *this << " (after assignment).\n"
                  << std::flush;
    return *this;
}

/* Subtract and assign:
 * self(a, b, c, d) -= q(a, b, c, d) ->
 * self(self.a - q.a, self.b - q.b, self.c - q.c, self.d - q.d). */
Quaternion& Quaternion::operator-=(const Quaternion& q)
{
    if(DEBUG_LEVEL > 4)
        std::cerr << "(II) Quaternion::operator-=(const Quaternion& q);\n"
                     "\tCalled with: " << q << ".\n"
                     "\tSelf: " << *this << " (before assignment).\n";
    val->setA(val->getA() - q.val->getA());
    val->setB(val->getB() - q.val->getB());
    val->setC(val->getC() - q.val->getC());
    val->setD(val->getD() - q.val->getD());
    if(DEBUG_LEVEL > 4)
        std::cerr << "\tSelf: " << *this << " (after assignment).\n"
                  << std::flush;
    return *this;
}

/* Multiply and assign:
 * self(a, b, c, d) *= q(a, b, c, d) ->
 * self(self.a * q.a - self.b * q.b - self.c * q.c - self.d * q.d,
 *      self.a * q.b + self.b * q.a + self.c * q.d - self.d * q.c,
 *      self.a * q.c - self.b * q.d + self.c * q.a + self.d * q.b,
 *      self.a * q.d + self.b * q.c - self.c * q.b + self.d * q.a) */
Quaternion& Quaternion::operator*=(const Quaternion& q)
{
    if(DEBUG_LEVEL > 4)
        std::cerr << "(II) Quaternion::operator*=(const Quaternion& q);\n"
                     "\tCalled with: " << q << ".\n"
                     "\tSelf: " << *this << " (before assignment).\n";
    Real a = val->getA();
    Real b = val->getB();
    Real c = val->getC();
    Real d = val->getD();
    Real q_a_val = q.val->getA();
    Real q_b_val = q.val->getB();
    Real q_c_val = q.val->getC();
    Real q_d_val = q.val->getD();
    val->setA(a * q_a_val - b * q_b_val - c * q_c_val - d * q_d_val);
    val->setB(a * q_b_val + b * q_a_val + c * q_d_val - d * q_c_val);
    val->setC(a * q_c_val - b * q_d_val + c * q_a_val + d * q_b_val);
    val->setD(a * q_d_val + b * q_c_val - c * q_b_val + d * q_a_val);
    if(DEBUG_LEVEL > 4)
        std::cerr << "\tSelf: " << *this << " (after assignment).\n"
                  << std::flush;
    return *this;
}

/* Division and assign:
 * self(a, b, c, d) /= q(a, b, c, d) ->
 * self((self * conj(q)) / (||q|| ^ 2)) */
Quaternion& Quaternion::operator/=(const Quaternion& q)
    throw (DivideByZeroException)
{
    if(DEBUG_LEVEL > 4)
        std::cerr << "(II) Quaternion::operator/=(const Quaternion& q);\n"
                     "\tCalled with: " << q << ".\n"
                     "\tSelf: " << *this << " (before assignment).\n";
    if(!q)
        throw DivideByZeroException();
    *this *= ::conj(q);
    *this /= ::det(q);
    if(DEBUG_LEVEL > 4)
        std::cerr << "\tSelf: " << *this << " (after assignment).\n"
                  << std::flush;
    return *this;
}

/* Add real and assign:
 * self(a, b, c, d) += r -> self(a + r, b, c, d); */
Quaternion& Quaternion::operator+=(const Real& r)
{
    if(DEBUG_LEVEL > 4)
        std::cerr << "(II) Quaternion::operator+=(const Real& r);\n"
                     "\tCalled with: " << r << ".\n"
                     "\tSelf: " << *this << " (before assignment).\n";
    val->setA(val->getA() + r);
    if(DEBUG_LEVEL > 4)
        std::cerr << "\tSelf: " << *this << " (after assignment).\n"
                  << std::flush;
    return *this;
}

/* Subtract real and assign:
 * self(a, b, c, d) -= r -> self(a - r, b, c, d); */
Quaternion& Quaternion::operator-=(const Real& r)
{
    if(DEBUG_LEVEL > 4)
        std::cerr << "(II) Quaternion::operator-=(const Real& r);\n"
                     "\tCalled with: " << r << ".\n"
                     "\tSelf: " << *this << " (before assignment).\n";
    val->setA(val->getA() - r);
    if(DEBUG_LEVEL > 4)
        std::cerr << "\tSelf: " << *this << " (after assignment).\n"
                  << std::flush;
    return *this;
}

/* Multiply by real and assign:
 * self(a, b, c, d) *= r -> self(r * a, r * b, r * c, r * d); */
Quaternion& Quaternion::operator*=(const Real& r)
{
    if(DEBUG_LEVEL > 4)
        std::cerr << "(II) Quaternion::operator*=(const Real& r);\n"
                     "\tCalled with: " << r << ".\n"
                     "\tSelf: " << *this << " (before assignment).\n";
    val->setA(val->getA() * r);
    val->setB(val->getB() * r);
    val->setC(val->getC() * r);
    val->setD(val->getD() * r);
    if(DEBUG_LEVEL > 4)
        std::cerr << "\tSelf: " << *this << " (after assignment).\n"
                  << std::flush;
    return *this;
}

/* Divide by real and assign:
 * self(a, b, c, d) /= r -> self(a / r, b / r, c / r, d / r); */
Quaternion& Quaternion::operator/=(const Real& r)
    throw (DivideByZeroException)
{
    if(DEBUG_LEVEL > 4)
        std::cerr << "(II) Quaternion::operator/=(const Real& r);\n"
                     "\tCalled with: " << r << ".\n"
                     "\tSelf: " << *this << " (before assignment).\n";
    if(r == RZERO)
        throw DivideByZeroException();
    val->setA(val->getA() / r);
    val->setB(val->getB() / r);
    val->setC(val->getC() / r);
    val->setD(val->getD() / r);
    if(DEBUG_LEVEL > 4)
        std::cerr << "\tSelf: " << *this << " (after assignment).\n"
                  << std::flush;
    return *this;
}


/* Bool casting.
 * `self` is `false` if and only if `self` == (0, 0, 0, 0).
 * `self` is `true` in all other cases. */
Quaternion::operator bool() const
{
    static const Quaternion zero = Quaternion(RZERO, RZERO, RZERO, RZERO);
    if(DEBUG_LEVEL > 4)
        std::cerr << "(II) Quaternion::operator bool() const;\n"
                     "\tSelf: " << *this << "\n."
                  << std::flush;
    return !(*this == zero);
}

/* Boolean negation.
 * Returns `false` if and only if given `Quaternion` casts itself to `true`. */
bool Quaternion::operator!() const {
    static const Quaternion zero = Quaternion(RZERO, RZERO, RZERO, RZERO);
    if(DEBUG_LEVEL > 4)
        std::cerr << "(II) Quaternion::operator!() const;\n"
                     "\tSelf: " << *this << "\n."
                  << std::flush;
    return (*this == zero);
}

/* Add:
 * self(a, b, c, d) + q(a, b, c, d) ->
 * (self.a + q.a, self.b + q.b, self.c + q.c, self.d + q.d). */
const Quaternion operator+(const Quaternion& p, const Quaternion& q)
{
    if(DEBUG_LEVEL > 4)
        std::cerr << "(II) "
                     "operator+(const Quaternion& p, const Quaternion& q);\n"
                     "\tCalled with (p): " << p << ",\n"
                     "\tCalled with (q): " << q << ".\n"
                  << std::flush;
    return Quaternion(p) += q;
}

/* Subtract:
 * self(a, b, c, d) - q(a, b, c, d) ->
 * (self.a - q.a, self.b - q.b, self.c - q.c, self.d - q.d). */
const Quaternion operator-(const Quaternion& p, const Quaternion& q)
{
    if(DEBUG_LEVEL > 4)
        std::cerr << "(II) "
                     "operator-(const Quaternion& p, const Quaternion& q);\n"
                     "\tCalled with (p): " << p << ",\n"
                     "\tCalled with (q): " << q << ".\n"
                  << std::flush;
    return Quaternion(p) -= q;
}

/* Multiply (hamilton product):
 * self(a, b, c, d) * q(a, b, c, d) ->
 * (self.a * q.a - self.b * q.b - self.c * q.c - self.d * q.d,
 *  self.a * q.b + self.b * q.a + self.c * q.d - self.d * q.c,
 *  self.a * q.c - self.b * q.d + self.c * q.a + self.d * q.b,
 *  self.a * q.d + self.b * q.c - self.c * q.b + self.d * q.a) */
const Quaternion operator*(const Quaternion& p, const Quaternion& q)
{
    if(DEBUG_LEVEL > 4)
        std::cerr << "(II) "
                     "operator*(const Quaternion& p, const Quaternion& q);\n"
                     "\tCalled with (p): " << p << ",\n"
                     "\tCalled with (q): " << q << ".\n"
                  << std::flush;
    return Quaternion(p) *= q;
}

/* Division:
 * self(a, b, c, d) / q(a, b, c, d) ->
 * (self * conj(q)) / (||q|| ^ 2) */
const Quaternion operator/(const Quaternion& p, const Quaternion& q)
    throw (DivideByZeroException)
{
    if(DEBUG_LEVEL > 4)
        std::cerr << "(II) "
                     "operator/(const Quaternion& p, const Quaternion& q);\n"
                     "\tCalled with (p): " << p << ",\n"
                     "\tCallew with (q): " << q << ".\n"
                  << std::flush;
    return Quaternion(p) /= q;
}

/* Add real to quaternion:
 * self(a, b, c, d) + r -> (a + r, b, c, d); */
const Quaternion operator+(const Quaternion& q, const Real& r)
{
    if(DEBUG_LEVEL > 4)
        std::cerr << "(II) "
                     "operator+(const Quaternion& q, const Real& r);\n"
                     "\tCalled with (q): " << q << ",\n"
                     "\tCalled with (r): " << r << ".\n"
                  << std::flush;
    return Quaternion(q) += r;
}

/* Add real to quaternion:
 * self(a, b, c, d) + r -> (a + r, b, c, d); */
const Quaternion operator+(const Real& r, const Quaternion& q)
{
    if(DEBUG_LEVEL > 4)
        std::cerr << "(II) "
                     "operator+(const Real& r, const Quaternion& q);\n"
                     "\tCalled with (r): " << r << ",\n"
                     "\tCalled with (q): " << q << ".\n"
                  << std::flush;
    return Quaternion(q) += r;
}

/* Subtract real from quaternion:
 * self(a, b, c, d) - r -> (a - r, b, c, d); */
const Quaternion operator-(const Quaternion& q, const Real& r)
{
    if(DEBUG_LEVEL > 4)
        std::cerr << "(II) "
                     "operator-(const Quaternion& q, const Real& r);\n"
                     "\tCalled with (q): " << q << ",\n"
                     "\tCalled with (r): " << r << ".\n"
                  << std::flush;
    return Quaternion(q) -= r;
}

/* Subtract real from quaternion:
 * self(a, b, c, d) - r -> (a - r, b, c, d); */
const Quaternion operator-(const Real& r, const Quaternion& q)
{
    if(DEBUG_LEVEL > 4)
        std::cerr << "(II) "
                     "operator+(const Real& r, const Quaternion& q);\n"
                     "\tCalled with (r): " << r << ",\n"
                     "\tCalled with (q): " << q << ".\n"
                  << std::flush;
    return (-q) += r;
}

/* Multiply quaternion by real:
 * self(a, b, c, d) * r -> (r * a, r * b, r * c, r * d); */
const Quaternion operator*(const Quaternion& q, const Real& r)
{
    if(DEBUG_LEVEL > 4)
        std::cerr << "(II) "
                     "operator*(const Quaternion& q, const Real& r);\n"
                     "\tCalled with (q): " << q << ",\n"
                     "\tCalled with (r): " << r << ".\n"
                  << std::flush;
    return Quaternion(q) *= r;
}

/* Multiply quaternion by real:
 * self(a, b, c, d) * r -> (r * a, r * b, r * c, r * d); */
const Quaternion operator*(const Real& r, const Quaternion& q)
{
    if(DEBUG_LEVEL > 4)
        std::cerr << "(II) "
                     "operator+(const Real& r, const Quaternion& q);\n"
                     "\tCalled with (r): " << r << ",\n"
                     "\tCalled with (q): " << q << ".\n"
                  << std::flush;
    return Quaternion(q) *= r;
}

/* Divide by real and assign:
 * self(a, b, c, d) /= r -> self(a / r, b / r, c / r, d / r); */
const Quaternion operator/(const Quaternion& q, const Real& r)
    throw (DivideByZeroException)
{
    if(DEBUG_LEVEL > 4)
        std::cerr << "(II) "
                     "operator/(const Quaternion& q, const Real& r);\n"
                     "\tCalled with (q): " << q << ",\n"
                     "\tCalled with (r): " << r << ".\n"
                  << std::flush;
    return Quaternion(q) /= r;
}

/* Divide by real and assign:
 * self(a, b, c, d) /= r -> self(a / r, b / r, c / r, d / r); */
const Quaternion operator/(const Real& r, const Quaternion& q)
    throw (DivideByZeroException)
{
    if(DEBUG_LEVEL > 4)
        std::cerr << "(II) "
                     "operator+(const Real& r, const Quaternion& q);\n"
                     "\tCalled with (r): " << r << ",\n"
                     "\tCalled with (q): " << q << ".\n"
                  << std::flush;
    return reciprocal(q) *= r;
}

/* Are same?
 * self(a, b, c, d) == q(a, b, c, d) if and only if
 * self.a == q.a AND
 * self.b == q.b AND
 * self.c == q.c AND
 * self.d == q.d. */
bool operator==(const Quaternion& p, const Quaternion& q)
{
    if(DEBUG_LEVEL > 4)
        std::cerr << "(II) "
                     "operator==(const Quaternion& p, const Quaternion& q);\n"
                     "\tCalled with (`p`): " << p << ",\n"
                     "\tCalled with (`q`): " << q << ".\n"
                  << std::flush;
    return (p.val->getA() == q.val->getA()) &&
           (p.val->getB() == q.val->getB()) &&
           (p.val->getC() == q.val->getC()) &&
           (p.val->getD() == q.val->getD());
}

/* Are not same?
 * self(a, b, c, d) != q(a, b, c, d) if and only if
 * self.a != q.a OR
 * self.b != q.b OR
 * self.c != q.c OR
 * self.d != q.d. */
bool operator!=(const Quaternion& p, const Quaternion& q)
{
    if(DEBUG_LEVEL > 4)
        std::cerr << "(II) "
                     "operator!=(const Quaternion& p, const Quaternion& q);\n"
                     "\tCalled with (`p`): " << p << ",\n"
                     "\tCalled with (`q`): " << q << ".\n"
                  << std::flush;
    return !(p == q);
}

/* Pushing quaternion `q` to output stream `os`. */
std::ostream& operator<<(std::ostream& os, const Quaternion& q)
{
    Real a_val = q.val->getA();
    Real b_val = q.val->getB();
    Real c_val = q.val->getC();
    Real d_val = q.val->getD();
    if(DEBUG_LEVEL > 4)
        std::cerr << "(II) "
                     "ostream& operator<<(ostream& os, const Quaternion& q);\n"
                     "\tCalled with: "
                  << a_val << " + " << b_val << "i + "
                  << c_val << "j + " << d_val << "k.\n"
                  << std::flush;
    os << a_val << " + "
       << b_val << "i + "
       << c_val << "j + "
       << d_val << "k";
    return os;
}

/* Returns real (scalar part) of `q` quaternion; that means:
 * (a, _, _, _) -> (a, 0, 0, 0). */
Quaternion re(const Quaternion& q)
{
    if(DEBUG_LEVEL > 4)
        std::cerr << "(II) re(const Quaternion& q);\n"
                     "\tCalled with: " << q << ".\n"
                  << std::flush;
    return Quaternion(q.r());
}

/* Returns pure imaginary (vector part) of `q` quaternion; that means:
 * (_, b, c, d) -> (0, b, c, d). */
Quaternion im(const Quaternion& q)
{
    if(DEBUG_LEVEL > 4)
        std::cerr << "(II) im(const Quaternion& q);\n"
                     "\tCalled with: " << q << ".\n"
                  << std::flush;
    return Quaternion(RZERO, q.i(), q.j(), q.k());
}

/* Returns conjugated quaternion `q`:
 * (a, b, c, d) -> (a, -b, -c, -d). */
Quaternion conj(const Quaternion& q)
{
    if(DEBUG_LEVEL > 4)
        std::cerr << "(II) conj(const Quaternion& q);\n"
                     "\tCalled with: " << q << ".\n"
                  << std::flush;
    return Quaternion(q.r(), -q.i(), -q.j(), -q.k());
}

/* Returns unit of quaternion `q`:
 * q(a, b, c, d) -> (a / ||q||, b / ||q||, c / ||q||, d / ||q||)
 * or (0., 0., 0., 0.) if q == (0., 0., 0., 0.)
 * where ||.|| is `norm` declared below. */
Quaternion unit(const Quaternion& q)
{
    if(DEBUG_LEVEL > 4)
        std::cerr << "(II) unit(const Quaternion& q);\n"
                     "\tCalled with: " << q << ".\n"
                  << std::flush;
    return q ? Quaternion(q) /= det(q) : q;
}

/* Returns determinant of given quaternion:
 * (a, b, c, d) -> a ^ 2 + b ^ 2 + c ^ 2 + d ^ 2. */
Real det(const Quaternion& q)
{
    if(DEBUG_LEVEL > 4)
        std::cerr << "(II) det(const Quaternion& q);\n"
                     "\tCalled with: " << q << ".\n"
                  << std::flush;
    Real a = q.r();
    Real b = q.i();
    Real c = q.j();
    Real d = q.k();
    return (a * a) + (b * b) + (c * c) + (d * d);
}

/* Returns norm of quaternion `q`:
 * (a, b, c, d) -> (a ^ 2 + b ^ 2 + c ^ 2 + d ^ 2) ^ 0.5. */
Real norm(const Quaternion& q)
{
    if(DEBUG_LEVEL > 4)
        std::cerr << "(II) norm(const Quaternion& q);\n"
                     "\tCalled with: " << q << ".\n"
                  << std::flush;
    return sqrt(det(q));
}

/* Returns reciprocal of a quaternion `q`:
 * q -> 1/q, that is:
 * q(a, b, c, d) -> (a, -b, -c, -d) / ||q||. */
Quaternion reciprocal(const Quaternion& q)
    throw (DivideByZeroException)
{
    if(DEBUG_LEVEL > 4)
        std::cerr << "(II) conj(const Quaternion& q);\n"
                     "\tCalled with: " << q << ".\n"
                  << std::flush;
    if(!q)
        throw DivideByZeroException();
    return Quaternion(q.r(), -q.i(), -q.j(), -q.k()) /= det(q);
}

/* Returns dot product of given imaginary quaternions:
 * (0, i1, j1, k1) . (0, i2, j2, k2) -> i1 * i2 + j1 * j2 + k1 * k2. */
Real dotProd(const Quaternion& p, const Quaternion& q)
{
    if(DEBUG_LEVEL > 4)
        std::cerr << "(II) "
                     "dotProd(const Quaternion& p, const Quaternion& q);\n"
                     "\tCalled with (`p`): " << p << ",\n"
                     "\tCalled with (`q`): " << q << ".\n"
                  << std::flush;
    if(DEBUG_LEVEL > 2)
        if(p.r() == RZERO)
            std::cerr << "(WW)\t`p` is not pure imaginary!\n" << std::flush;
        if(q.r() == RZERO)
            std::cerr << "(WW)\t`q` is not pure imaginary!\n" << std::flush;
    return (p.i() * q.i()) + (p.j() * q.j()) + (p.k() * q.k());
}

/* Returns cross product of given imaginary quaternions:
 * (0, b1, c1, d1) x (0, b2, c2, d2) ->
 * (0, c1 * d2 - d1 * c2, d1 * b2 - b1 * d2, b1 * c2 - c1 * b2) */
Quaternion crossProd(const Quaternion& p, const Quaternion& q)
{
    if(DEBUG_LEVEL > 4)
        std::cerr << "(II) "
                     "crossProd(const Quaternion& p, const Quaternion& q);\n"
                     "\tCalled with (`p`): " << p << ",\n"
                     "\tCalled with (`q`): " << q << ".\n"
                  << std::flush;
    if(DEBUG_LEVEL > 2)
        if(p.r() == RZERO)
            std::cerr << "(WW)\t`p` is not pure imaginary!\n" << std::flush;
        if(q.r() == RZERO)
            std::cerr << "(WW)\t`q` is not pure imaginary!\n" << std::flush;
    /* `p`: */
    Real b1 = p.i();
    Real c1 = p.j();
    Real d1 = p.k();
    /* `q`: */
    Real b2 = q.i();
    Real c2 = q.j();
    Real d2 = q.k();
    return Quaternion(RZERO,
                      (c1 * d2) - (d1 * c2),
                      (d1 * b2) - (b1 * d2),
                      (b1 * c2) - (c1 * b2));
}

/* Returns exponent of given quaternion (q = (a, b, c, d)):
 * exp(q) = (e ^ a) * (cos||v|| + Uv * sin||v||),
 * where `v` is pure imaginary of `q` (v = im(q)),
 * and `Uv` is unit of `v` (Uv = unit(v)). */
Quaternion exp(const Quaternion& q)
{
    if(DEBUG_LEVEL > 4)
        std::cerr << "(II) exp(const Quaternion& q);\n"
                     "\tCalled with: " << q << ".\n"
                  << std::flush;
    Quaternion v = im(q);
    Quaternion uv = unit(v);
    Real norm_v = norm(v);
    return (uv * sin(norm_v) + cos(norm_v)) * exp(q.r());
}

/* Returns natural logarithm of given quaternion (q = (a, b, c, d)):
 * log(q) = ln||q|| + Uv * arccos(a / ||q||),
 * where `v` is pure imaginary of `q` (v = im(q)),
 * and `Uv` is unit of `v` (Uv = unit(v)). */
Quaternion log(const Quaternion& q)
{
    if(DEBUG_LEVEL > 4)
        std::cerr << "(II) ln(const Quaternion& q);\n"
                     "\tCalled with: " << q << ".\n"
                  << std::flush;
    Quaternion v = im(q);
    Quaternion uv = unit(q);
    Real norm_q = norm(q);
    return log(norm_q) + uv * acos(q.r() / norm_q);
}

/* Returns `i` quaternion (0, 1, 0, 0). */
Quaternion I()
{
    if(DEBUG_LEVEL > 4)
        std::cerr << "(II) I().\n" << std::flush;
    return Quaternion(RZERO, RONE, RZERO, RZERO);
}

/* Returns `j` quaternion (0, 0, 1, 0). */
Quaternion J()
{
    if(DEBUG_LEVEL > 4)
        std::cerr << "(II) J().\n" << std::flush;
    return Quaternion(RZERO, RZERO, RONE, RZERO);
}

/* Returns `k` quaternion (0, 0, 0, 1). */
Quaternion K()
{
    if(DEBUG_LEVEL > 4)
        std::cerr << "(II) K().\n" << std::flush;
    return Quaternion(RZERO, RZERO, RZERO, RONE);
}
