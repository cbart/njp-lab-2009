/** author: Cezary Bartoszuk <cbart@students.mimuw.edu.pl> *
 *    user: cb277617@students.mimuw.edu.pl                 */

#include <cmath>
#include <iostream>
#include <stdexcept>
#include "./debug_tools.h"
#include "./quaternion.h"

/** DEBUGLEVEL:                         *
 *  0 - no debug;                       *
 *  1 - assertions ON;                  *
 *  2 - 1 and info, warnings, errors;   */
/** To manage turning assertions On/Off
 *  you have to toggle -D NDEBUG option
 *  while compiling. */

#ifdef DEBUGLEVEL
    const DebugLevel DEBUG_LEVEL = DEBUGLEVEL;
#else
    const DebugLevel DEBUG_LEVEL = 0;
#endif

const String DIAG_PROG_NAME = "Quaternion";

const DebugLevel ERROR_DEBUG_LEVEL = 2;
const DebugLevel WARNING_DEBUG_LEVEL = 2;
const DebugLevel INFO_DEBUG_LEVEL = 2;

/* Exception raised when division by zero is encountered. */
DivideByZeroException::DivideByZeroException()
  : std::runtime_error("(EX) Attempted to divide by zero!\n")
{
    logErr() << "DivideByZero::DivideByZero() thrown!\n"
        << std::flush;
}

/* Inner `Quaternion`s value container constructor. */
Quaternion::QContainer::QContainer()
  : a_val(0.),
    b_val(0.),
    c_val(0.),
    d_val(0.)
{
    logInfo() << "Quaternion::QContainer::QContainer();\n"
        << std::flush;
}

/* Inner `Quaternion`s value container constructor. */
Quaternion::QContainer::QContainer(Real a, Real b, Real c, Real d)
  : a_val(a),
    b_val(b),
    c_val(c),
    d_val(d)
{
    logInfo() << "Quaternion::QContainer::QContainer"
        "(Real a, Real b, Real c, Real d);\n" << std::flush;
}

/* Inner `Quaternion`s value container copy constructor. */
Quaternion::QContainer::QContainer(const IQContainer& cnt)
  : a_val(cnt.getA()),
    b_val(cnt.getB()),
    c_val(cnt.getC()),
    d_val(cnt.getD())
{
    logInfo() << "Quaternion::QContainer::QContainer"
        "(const IQContainer& cnt);\n" << std::flush;
}

/* `QContainer`s destructor. */
Quaternion::QContainer::~QContainer()
{
    logInfo() << "Quaternion::QContainer::~QContainer();\n" << std::flush;
}

Quaternion::QContainer& Quaternion::QContainer::operator=(const QContainer& cnt)
{
    logInfo() << "Quaterinon::QContainer::operator=(const QContainer& cnt);\n"
        << std::flush;
    a_val = cnt.a_val;
    b_val = cnt.b_val;
    c_val = cnt.c_val;
    d_val = cnt.d_val;
    return *this;
}

Quaternion::QContainer* Quaternion::QContainer::clone() const
{
    logInfo() << "Quaternion::QContainer* Quaternion::QContainer::clone();\n"
        << std::flush;
    return new QContainer(*this);
}

Real Quaternion::QContainer::getA() const
{
    return a_val;
}

Real Quaternion::QContainer::getB() const
{
    return b_val;
}

Real Quaternion::QContainer::getC() const
{
    return c_val;
}

Real Quaternion::QContainer::getD() const
{
    return d_val;
}

void Quaternion::QContainer::setA(Real a)
{
    a_val = a;
}

void Quaternion::QContainer::setB(Real b)
{
    b_val = b;
}

void Quaternion::QContainer::setC(Real c)
{
    c_val = c;
}

void Quaternion::QContainer::setD(Real d)
{
    d_val = d;
}

/* Creates `Quaternion` with given `QContainer`. */
Quaternion::Quaternion(IQContainer* cnt)
    : val(cnt)
{
    logInfo() << "Quaternion::Quaternion(IQContainer* cnt);\n" << std::flush;
}

/* Creates `zero` quaternion (0, 0, 0, 0). */
Quaternion::Quaternion()
    : val(new Quaternion::QContainer(0., 0., 0., 0.))
{
    logInfo() << "Quaternion::Quaternion();\n" << std::flush;
}

/* Creates real quaternion (a, 0, 0, 0). */
Quaternion::Quaternion(Real a)
  : val(new Quaternion::QContainer(a, 0., 0., 0.))
{
    logInfo() << "Quaternion::Quaternion(Real a);\n" << std::flush;
}

/* Creates quaternion which behaves like complex number
 * with Re = `a` and Im = `b`: (a, b, 0, 0). */
Quaternion::Quaternion(Real a, Real b)
  : val(new Quaternion::QContainer(a, b, 0., 0.))
{
    logInfo() << "Quaternion::Quaternion(Real a, Real b);\n" << std::flush;
}

/* Creates common quaternion (a, b, c, d). */
Quaternion::Quaternion(Real a, Real b, Real c, Real d)
  : val(new Quaternion::QContainer(a, b, c, d))
{
    logInfo() << "Quaternion::Quaternion(Real a, Real b, Real c, Real d);\n"
        << std::flush;
}

/* Creates copy of quaternion `q`. */
Quaternion::Quaternion(const Quaternion& q)
  : val(q.val->clone())
{
    logInfo() << "Quaternion::Quaternion(const Quaternion&);\n"
        "\tCalled with: " << q << ".\n" << std::flush;
}

/* Destructor. */
Quaternion::~Quaternion()
{
    logInfo() << "Quaternion::~Quaternion();\n" << std::flush;
    delete val;
}

/* Returns quaternion's first coordinate. */
Real Quaternion::r() const
{
    return val->getA();
}

/* Returns quaternion's second coordinate. */
Real Quaternion::i() const {
    return val->getB();
}

/* Returns quaternion's third coordinate. */
Real Quaternion::j() const {
    return val->getC();
}

/* Returns quaternion's fourth coordinate. */
Real Quaternion::k() const
{
    return val->getD();
}

/* Assigns given quaternion to `self`. */
Quaternion& Quaternion::operator=(const Quaternion& q)
{
    logInfo() << "Quaternion::operator=(const Quaternion&);\n"
        "\tLeft:  " << *this << ";\n"
        "\tRight: " << q << ".\n"
        << std::flush;
    if(this == &q)
        logWarn() << "Requested self-assignment.\n" << std::flush;
    else {
        val->setA(q.val->getA());
        val->setB(q.val->getB());
        val->setC(q.val->getC());
        val->setD(q.val->getD());
    }
    return *this;
}

/* Unary minus (prefix):
 * (a, b, c, d) -> (-a, -b, -c, -d). */
Quaternion Quaternion::operator-() const
{
    logInfo() << "Quaternion::operator-() const;\n" << std::flush;
    return Quaternion
        (-(val->getA()), -(val->getB()), -(val->getC()), -(val->getD()));
}

/* Add and assign:
 * self(a, b, c, d) += q(a, b, c, d) ->
 * self(self.a + q.a, self.b + q.b, self.c + q.c, self.d + q.d). */
Quaternion& Quaternion::operator+=(const Quaternion& q)
{
    logInfo() << "Quaternion::operator+=(const Quaternion& q);\n"
        "\tCalled with: " << q << ".\n"
        "\tSelf: " << *this << " (before assignment).\n" << std::flush;
    val->setA(val->getA() + q.val->getA());
    val->setB(val->getB() + q.val->getB());
    val->setC(val->getC() + q.val->getC());
    val->setD(val->getD() + q.val->getD());
    logInfo() << "\tSelf: " << *this << " (after assignment).\n" << std::flush;
    return *this;
}

/* Subtract and assign:
 * self(a, b, c, d) -= q(a, b, c, d) ->
 * self(self.a - q.a, self.b - q.b, self.c - q.c, self.d - q.d). */
Quaternion& Quaternion::operator-=(const Quaternion& q)
{
    logInfo() << "Quaternion::operator-=(const Quaternion& q);\n"
        "\tCalled with: " << q << ".\n"
        "\tSelf: " << *this << " (before assignment).\n" << std::flush;
    val->setA(val->getA() - q.val->getA());
    val->setB(val->getB() - q.val->getB());
    val->setC(val->getC() - q.val->getC());
    val->setD(val->getD() - q.val->getD());
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
    logInfo() << "Quaternion::operator*=(const Quaternion& q);\n"
        "\tCalled with: " << q << ".\n"
        "\tSelf: " << *this << " (before assignment).\n" << std::flush;
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
    return *this;
}

/* Division and assign:
 * self(a, b, c, d) /= q(a, b, c, d) ->
 * self((self * conj(q)) / (||q|| ^ 2)) */
Quaternion& Quaternion::operator/=(const Quaternion& q)
    throw (DivideByZeroException)
{
    logInfo() << "Quaternion::operator/=(const Quaternion& q);\n"
        "\tCalled with: " << q << ".\n"
        "\tSelf: " << *this << " (before assignment).\n";
    if(!q)
        throw DivideByZeroException();
    *this *= ::conj(q);
    *this /= ::det(q);
    return *this;
}

/* Add real and assign:
 * self(a, b, c, d) += r -> self(a + r, b, c, d); */
Quaternion& Quaternion::operator+=(const Real& r)
{
    logInfo() << "Quaternion::operator+=(const Real& r);\n"
        "\tCalled with: " << r << ".\n"
        "\tSelf: " << *this << " (before assignment).\n";
    val->setA(val->getA() + r);
    return *this;
}

/* Subtract real and assign:
 * self(a, b, c, d) -= r -> self(a - r, b, c, d); */
Quaternion& Quaternion::operator-=(const Real& r)
{
    logInfo() << "Quaternion::operator-=(const Real& r);\n"
        "\tCalled with: " << r << ".\n"
        "\tSelf: " << *this << " (before assignment).\n";
    val->setA(val->getA() - r);
    return *this;
}

/* Multiply by real and assign:
 * self(a, b, c, d) *= r -> self(r * a, r * b, r * c, r * d); */
Quaternion& Quaternion::operator*=(const Real& r)
{
    logInfo() << "Quaternion::operator*=(const Real& r);\n"
        "\tCalled with: " << r << ".\n"
        "\tSelf: " << *this << " (before assignment).\n";
    val->setA(val->getA() * r);
    val->setB(val->getB() * r);
    val->setC(val->getC() * r);
    val->setD(val->getD() * r);
    return *this;
}

/* Divide by real and assign:
 * self(a, b, c, d) /= r -> self(a / r, b / r, c / r, d / r); */
Quaternion& Quaternion::operator/=(const Real& r)
    throw (DivideByZeroException)
{
    logInfo() << "Quaternion::operator/=(const Real& r);\n"
        "\tCalled with: " << r << ".\n"
        "\tSelf: " << *this << " (before assignment).\n";
    if(r == RZERO)
        throw DivideByZeroException();
    val->setA(val->getA() / r);
    val->setB(val->getB() / r);
    val->setC(val->getC() / r);
    val->setD(val->getD() / r);
    return *this;
}


/* Bool testing.
 * `self` is `false` if and only if `self` == (0, 0, 0, 0).
 * `self` is `true` in all other cases. */
Boolean Quaternion::booleanTest() const
{
    return (r() != RZERO) || (i() != RZERO) || (j() != RZERO) || (k() != RZERO);
}

/* Add:
 * self(a, b, c, d) + q(a, b, c, d) ->
 * (self.a + q.a, self.b + q.b, self.c + q.c, self.d + q.d). */
const Quaternion operator+(const Quaternion& p, const Quaternion& q)
{
    logInfo() << "operator+(const Quaternion& p, const Quaternion& q);\n"
        "\tCalled with (p): " << p << ",\n"
        "\tCalled with (q): " << q << ".\n" << std::flush;
    return Quaternion(p) += q;
}

/* Subtract:
 * self(a, b, c, d) - q(a, b, c, d) ->
 * (self.a - q.a, self.b - q.b, self.c - q.c, self.d - q.d). */
const Quaternion operator-(const Quaternion& p, const Quaternion& q)
{
    logInfo() << "operator-(const Quaternion& p, const Quaternion& q);\n"
        "\tCalled with (p): " << p << ",\n"
        "\tCalled with (q): " << q << ".\n" << std::flush;
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
    logInfo() << "operator*(const Quaternion& p, const Quaternion& q);\n"
        "\tCalled with (p): " << p << ",\n"
        "\tCalled with (q): " << q << ".\n" << std::flush;
    return Quaternion(p) *= q;
}

/* Division:
 * self(a, b, c, d) / q(a, b, c, d) ->
 * (self * conj(q)) / (||q|| ^ 2) */
const Quaternion operator/(const Quaternion& p, const Quaternion& q)
    throw (DivideByZeroException)
{
    logInfo() << "operator/(const Quaternion& p, const Quaternion& q);\n"
        "\tCalled with (p): " << p << ",\n"
        "\tCallew with (q): " << q << ".\n" << std::flush;
    return Quaternion(p) /= q;
}

/* Add real to quaternion:
 * self(a, b, c, d) + r -> (a + r, b, c, d); */
const Quaternion operator+(const Quaternion& q, const Real& r)
{
    logInfo() << "operator+(const Quaternion& q, const Real& r);\n"
        "\tCalled with (q): " << q << ",\n"
        "\tCalled with (r): " << r << ".\n" << std::flush;
    return Quaternion(q) += r;
}

/* Add real to quaternion:
 * self(a, b, c, d) + r -> (a + r, b, c, d); */
const Quaternion operator+(const Real& r, const Quaternion& q)
{
    logInfo() << "operator+(const Real& r, const Quaternion& q);\n"
        "\tCalled with (r): " << r << ",\n"
        "\tCalled with (q): " << q << ".\n" << std::flush;
    return Quaternion(q) += r;
}

/* Subtract real from quaternion:
 * self(a, b, c, d) - r -> (a - r, b, c, d); */
const Quaternion operator-(const Quaternion& q, const Real& r)
{
    logInfo() << "operator-(const Quaternion& q, const Real& r);\n"
        "\tCalled with (q): " << q << ",\n"
        "\tCalled with (r): " << r << ".\n" << std::flush;
    return Quaternion(q) -= r;
}

/* Subtract real from quaternion:
 * self(a, b, c, d) - r -> (a - r, b, c, d); */
const Quaternion operator-(const Real& r, const Quaternion& q)
{
    logInfo() << "operator+(const Real& r, const Quaternion& q);\n"
        "\tCalled with (r): " << r << ",\n"
        "\tCalled with (q): " << q << ".\n" << std::flush;
    return (-q) += r;
}

/* Multiply quaternion by real:
 * self(a, b, c, d) * r -> (r * a, r * b, r * c, r * d); */
const Quaternion operator*(const Quaternion& q, const Real& r)
{
    logInfo() << "operator*(const Quaternion& q, const Real& r);\n"
        "\tCalled with (q): " << q << ",\n"
        "\tCalled with (r): " << r << ".\n" << std::flush;
    return Quaternion(q) *= r;
}

/* Multiply quaternion by real:
 * self(a, b, c, d) * r -> (r * a, r * b, r * c, r * d); */
const Quaternion operator*(const Real& r, const Quaternion& q)
{
    logInfo() << "operator+(const Real& r, const Quaternion& q);\n"
        "\tCalled with (r): " << r << ",\n"
        "\tCalled with (q): " << q << ".\n" << std::flush;
    return Quaternion(q) *= r;
}

/* Divide by real and assign:
 * self(a, b, c, d) /= r -> self(a / r, b / r, c / r, d / r); */
const Quaternion operator/(const Quaternion& q, const Real& r)
    throw (DivideByZeroException)
{
    logInfo() << "operator/(const Quaternion& q, const Real& r);\n"
        "\tCalled with (q): " << q << ",\n"
        "\tCalled with (r): " << r << ".\n" << std::flush;
    return Quaternion(q) /= r;
}

/* Divide by real and assign:
 * self(a, b, c, d) /= r -> self(a / r, b / r, c / r, d / r); */
const Quaternion operator/(const Real& r, const Quaternion& q)
    throw (DivideByZeroException)
{
    logInfo() << "operator+(const Real& r, const Quaternion& q);\n"
        "\tCalled with (r): " << r << ",\n"
        "\tCalled with (q): " << q << ".\n" << std::flush;
    return reciprocal(q) * r;
}

/* Are same?
 * self(a, b, c, d) == q(a, b, c, d) if and only if
 * self.a == q.a AND
 * self.b == q.b AND
 * self.c == q.c AND
 * self.d == q.d. */
inline bool equal(Real a, Real b) {
    static const Real EPSILON = 0.000000001;
    return (a - b < EPSILON) && (-EPSILON < a - b);
}
bool operator==(const Quaternion& p, const Quaternion& q)
{

    logInfo() << "operator==(const Quaternion& p, const Quaternion& q);\n"
        "\tCalled with (`p`): " << p << ",\n"
        "\tCalled with (`q`): " << q << ".\n" << std::flush;
    //if((p.val->getA() == q.val->getA()) &&
           //(p.val->getB() == q.val->getB()) &&
           //(p.val->getC() == q.val->getC()) &&
           //(p.val->getD() == q.val->getD()))
        //logInfo() << "-> equal.\n" << std::flush;
    //else
        //logInfo() << "-> not equal.\n" << std::flush;
    //return (p.val->getA() == q.val->getA()) &&
           //(p.val->getB() == q.val->getB()) &&
           //(p.val->getC() == q.val->getC()) &&
           //(p.val->getD() == q.val->getD());
    return (equal(p.val->getA(), q.val->getA()) &&
        equal(p.val->getB(), q.val->getB()) &&
        equal(p.val->getC(), q.val->getC()) &&
        equal(p.val->getD(), q.val->getD()));
}

/* Are not same?
 * self(a, b, c, d) != q(a, b, c, d) if and only if
 * self.a != q.a OR
 * self.b != q.b OR
 * self.c != q.c OR
 * self.d != q.d. */
bool operator!=(const Quaternion& p, const Quaternion& q)
{
    logInfo() << "operator!=(const Quaternion& p, const Quaternion& q);\n"
        "\tCalled with (`p`): " << p << ",\n"
        "\tCalled with (`q`): " << q << ".\n" << std::flush;
    return !(p == q);
}

/* Pushing quaternion `q` to output stream `os`. */
std::ostream& operator<<(std::ostream& os, const Quaternion& q)
{
    Real a_val = q.val->getA();
    Real b_val = q.val->getB();
    Real c_val = q.val->getC();
    Real d_val = q.val->getD();
    os << a_val << " + "
       << b_val << "i + "
       << c_val << "j + "
       << d_val << "k";
    return os;
}

/* Returns real (scalar part) of `q` quaternion; that means:
 * (a, _, _, _) -> (a, 0, 0, 0). */
const Quaternion re(const Quaternion& q)
{
    logInfo() << "re(const Quaternion& q);\n"
        "\tCalled with: " << q << ".\n" << std::flush;
    return Quaternion(q.r());
}

/* Returns pure imaginary (vector part) of `q` quaternion; that means:
 * (_, b, c, d) -> (0, b, c, d). */
const Quaternion im(const Quaternion& q)
{
    logInfo() << "im(const Quaternion& q);\n"
        "\tCalled with: " << q << ".\n" << std::flush;
    return Quaternion(RZERO, q.i(), q.j(), q.k());
}

/* Returns conjugated quaternion `q`:
 * (a, b, c, d) -> (a, -b, -c, -d). */
const Quaternion conj(const Quaternion& q)
{
    logInfo() << "conj(const Quaternion& q);\n"
        "\tCalled with: " << q << ".\n" << std::flush;
    return Quaternion(q.r(), -q.i(), -q.j(), -q.k());
}

/* Returns unit of quaternion `q`:
 * q(a, b, c, d) -> (a / ||q||, b / ||q||, c / ||q||, d / ||q||)
 * or (0., 0., 0., 0.) if q == (0., 0., 0., 0.)
 * where ||.|| is `norm` declared below. */
const Quaternion unit(const Quaternion& q)
{
    logInfo() << "unit(const Quaternion& q);\n"
        "\tCalled with: " << q << ".\n" << std::flush;
    return q ? Quaternion(q) /= det(q) : q;
}

/* Returns determinant of given quaternion:
 * (a, b, c, d) -> a ^ 2 + b ^ 2 + c ^ 2 + d ^ 2. */
const Real det(const Quaternion& q)
{
    logInfo() << "det(const Quaternion& q);\n"
        "\tCalled with: " << q << ".\n" << std::flush;
    Real a = q.r();
    Real b = q.i();
    Real c = q.j();
    Real d = q.k();
    return (a * a) + (b * b) + (c * c) + (d * d);
}

/* Returns norm of quaternion `q`:
 * (a, b, c, d) -> (a ^ 2 + b ^ 2 + c ^ 2 + d ^ 2) ^ 0.5. */
const Real norm(const Quaternion& q)
{
    logInfo() << "norm(const Quaternion& q);\n"
        "\tCalled with: " << q << ".\n" << std::flush;
    return sqrt(det(q));
}

/* Returns reciprocal of a quaternion `q`:
 * q -> 1/q, that is:
 * q(a, b, c, d) -> (a, -b, -c, -d) / ||q||. */
const Quaternion reciprocal(const Quaternion& q)
    throw (DivideByZeroException)
{
    logInfo() << "conj(const Quaternion& q);\n"
        "\tCalled with: " << q << ".\n" << std::flush;
    if(!q)
        throw DivideByZeroException();
    return Quaternion(q.r(), -q.i(), -q.j(), -q.k()) /= det(q);
}

/* Returns dot product of given imaginary quaternions:
 * (0, i1, j1, k1) . (0, i2, j2, k2) -> i1 * i2 + j1 * j2 + k1 * k2. */
const Real dotProd(const Quaternion& p, const Quaternion& q)
{
    logInfo() << "dotProd(const Quaternion& p, const Quaternion& q);\n"
        "\tCalled with (`p`): " << p << ",\n"
        "\tCalled with (`q`): " << q << ".\n" << std::flush;
    if(p.r() == RZERO)
        logWarn() << "`p` is not pure imaginary!\n" << std::flush;
    if(q.r() == RZERO)
        logWarn() << "`q` is not pure imaginary!\n" << std::flush;
    return (p.i() * q.i()) + (p.j() * q.j()) + (p.k() * q.k());
}

/* Returns cross product of given imaginary quaternions:
 * (0, b1, c1, d1) x (0, b2, c2, d2) ->
 * (0, c1 * d2 - d1 * c2, d1 * b2 - b1 * d2, b1 * c2 - c1 * b2) */
const Quaternion crossProd(const Quaternion& p, const Quaternion& q)
{
    logInfo() << "crossProd(const Quaternion& p, const Quaternion& q);\n"
        "\tCalled with (`p`): " << p << ",\n"
        "\tCalled with (`q`): " << q << ".\n" << std::flush;
    if(p.r() == RZERO)
        logWarn() << "`p` is not pure imaginary!\n" << std::flush;
    if(q.r() == RZERO)
        logWarn() << "`q` is not pure imaginary!\n" << std::flush;
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
const Quaternion exp(const Quaternion& q)
{
    logInfo() << "exp(const Quaternion& q);\n"
        "\tCalled with: " << q << ".\n" << std::flush;
    Quaternion v = im(q);
    Quaternion uv = unit(v);
    Real norm_v = norm(v);
    return (uv * sin(norm_v) + cos(norm_v)) * exp(q.r());
}

/* Returns natural logarithm of given quaternion (q = (a, b, c, d)):
 * log(q) = ln||q|| + Uv * arccos(a / ||q||),
 * where `v` is pure imaginary of `q` (v = im(q)),
 * and `Uv` is unit of `v` (Uv = unit(v)). */
const Quaternion log(const Quaternion& q)
{
    logInfo() << "ln(const Quaternion& q);\n"
        "\tCalled with: " << q << ".\n" << std::flush;
    Quaternion v = im(q);
    Quaternion uv = unit(q);
    Real norm_q = norm(q);
    return log(norm_q) + uv * acos(q.r() / norm_q);
}

const Quaternion pow(const Quaternion& q, UInteger i)
{
    if(i == 0)
        return Quaternion(RONE);
    UInteger power = i - 1;
    Quaternion q_pow = Quaternion(q);
    /* niezmiennik: pow(q,i) = q_pow * q ^ power; */
    while(power > 0) {
        if(power % 2 == 0) {
            q_pow *= q_pow;
            power /= 2;
        }
        else {
            q_pow *= q;
            power -= 1;
        }
    }
    return q_pow;
}

/* Returns `i` quaternion (0, 1, 0, 0). */
const Quaternion I()
{
    return Quaternion(RZERO, RONE, RZERO, RZERO);
}

/* Returns `j` quaternion (0, 0, 1, 0). */
const Quaternion J()
{
    return Quaternion(RZERO, RZERO, RONE, RZERO);
}

/* Returns `k` quaternion (0, 0, 0, 1). */
const Quaternion K()
{
    return Quaternion(RZERO, RZERO, RZERO, RONE);
}

