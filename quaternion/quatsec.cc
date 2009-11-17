#include <iostream>
#include <stdexcept>
#include <map>

#include "./debug_tools.h"
#include "./quatsec.h"
#include "./quaternion.h"

#ifdef DEBUGLEVEL
    const DebugLevel DEBUG_LEVEL = DEBUGLEVEL;
#else
    const DebugLevel DEBUG_LEVEL = 0;
#endif

typedef QuaternionSequence::ProxyQContainer ProxyQContainer;
typedef QuaternionSequence::size_type size_type;

const String DIAG_PROG_NAME = "Quatsec";
const DebugLevel ERROR_DEBUG_LEVEL = 2;
const DebugLevel WARNING_DEBUG_LEVEL = 2;
const DebugLevel INFO_DEBUG_LEVEL = 2;

ProxyQContainer::ProxyQContainer
    (QuaternionSequence::size_type id,
     QuaternionSequence& qs)
  : index(id),
    qs(&qs)
{
    logInfo() << "QuaternionSequece::ProxyQContainer::"
        "ProxyQContainer(QuaternionSequence::size_type id);\n" << std::flush;
}

ProxyQContainer::ProxyQContainer(const QuaternionSequence::ProxyQContainer& cnt)
  : index(cnt.index),
    qs(cnt.qs)
{
    logInfo() << "QuaternionSequence::ProxyQContainer::"
        "ProxyQContainer(QuaternionSequence::ProxyQContainer& cnt);\n"
        << std::flush;
}

ProxyQContainer& ProxyQContainer::operator=
    (const ProxyQContainer& cnt)
{
    index = cnt.index;
    qs = cnt.qs;
    return *this;
}

ProxyQContainer::~ProxyQContainer()
{
    logInfo() << "QuaternionSequece::ProxyQContainer::~ProxyQContainer();\n"
        << std::flush;
}

ProxyQContainer* ProxyQContainer::clone() const
{
    logInfo() << "QuaternionSequence::ProxyQContainer::clone();\n"
        << std::flush;
    return new ProxyQContainer(index, *qs);
}

Real ProxyQContainer::getA() const
{
    std::map<size_type, Quaternion>::iterator iter;
    iter = qs->seq.find(index);
    if(iter != qs->seq.end())
        return iter->second.r();
    else
        return RZERO;
}

Real ProxyQContainer::getB() const
{
    std::map<size_type, Quaternion>::iterator iter;
    iter = qs->seq.find(index);
    if(iter != qs->seq.end())
        return iter->second.i();
    else
        return RZERO;
}

Real ProxyQContainer::getC() const
{
    std::map<size_type, Quaternion>::iterator iter;
    iter = qs->seq.find(index);
    if(iter != qs->seq.end())
        return iter->second.j();
    else
        return RZERO;
}

Real ProxyQContainer::getD() const
{
    std::map<size_type, Quaternion>::iterator iter;
    iter = qs->seq.find(index);
    if(iter != qs->seq.end())
        return iter->second.k();
    else
        return RZERO;
}

void ProxyQContainer::setA(Real a)
{
    std::map<size_type, Quaternion>::iterator iter;
    iter = qs->seq.find(index);
    if(iter == qs->seq.end())
        qs->seq[index] = Quaternion();
    qs->seq[index].val->setA(a);
}

void ProxyQContainer::setB(Real b)
{
    std::map<size_type, Quaternion>::iterator iter;
    iter = qs->seq.find(index);
    if(iter == qs->seq.end())
        qs->seq[index] = Quaternion();
    qs->seq[index].val->setB(b);
}

void ProxyQContainer::setC(Real c)
{
    std::map<size_type, Quaternion>::iterator iter;
    iter = qs->seq.find(index);
    if(iter == qs->seq.end())
        qs->seq[index] = Quaternion();
    qs->seq[index].val->setC(c);
}

void ProxyQContainer::setD(Real d)
{
    if(qs->seq.find(index) == qs->seq.end())
        qs->seq[index] = Quaternion();
    qs->seq[index].val->setD(d);
}

Quaternion& QuaternionSequence::getQuaternion(size_type id)
{
    if(seq.find(id) == seq.end())
        seq[id] = Quaternion();
    return seq[id];
}

Quaternion& QuaternionSequence::setQuaternion
    (size_type id, const Quaternion& q)
{
    logInfo() << "QuaternionSequence::setQuaternion"
        "(size_type id, Quaternion q);\n"
        "\tCalled with (id): " << id << ",\n"
        "\tCalled with (q):  " << q << ".\n" << std::flush;
    seq[id] = Quaternion(q);
    return seq[id];
}

QuaternionSequence::QuaternionSequence()
  : seq(std::map<size_type, Quaternion>())
{
    logInfo() << "QuaternionSequence::QuaternionSequence();\n"
        << std::flush;
}

QuaternionSequence::QuaternionSequence
    (const std::map<size_type, Quaternion>& m_seq)
  : seq(m_seq)
{
    logInfo() << "QuaternionSequence::QuaternionSequence"
        "(const std::map<QuaternionSequence::size_type, Quaternion>);\n"
        << std::flush;
}

QuaternionSequence::QuaternionSequence
    (const std::vector<Quaternion>& v_seq)
  : seq(std::map<size_type, Quaternion>())
{
    logInfo() << "QuaternionSequence::QuaternionSequence"
        "(const std::vector<Quaternion>& v_seq);\n" << std::flush;
    size_type i = 0;
    for(std::vector<Quaternion>::const_iterator iter = v_seq.begin();
            iter < v_seq.end();
            iter ++)
        seq[i ++] = *iter;
}

QuaternionSequence::QuaternionSequence
    (const QuaternionSequence& qs)
  : seq(std::map<size_type, Quaternion>(qs.seq))
{
    logInfo() << "QuaternionSequence::QuaternionSequence"
        "(const QuaternionSequence& qs);\n" << std::flush;
}

QuaternionSequence::~QuaternionSequence()
{
    logInfo() << "QuaternionSequence::~QuaternionSequence();" << std::flush;
}

































