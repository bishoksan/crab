#pragma once 

#include <iosfwd>
#include <stdarg.h>
#include <boost/shared_ptr.hpp>
#include <boost/make_shared.hpp>
#include <boost/optional.hpp>
#include <boost/noncopyable.hpp>

/* Basic type definitions */

namespace crab {
  
  // An adaptor for std::ostream that avoids polluting all crab header
  // files with iostream stuff
  class crab_os: boost::noncopyable {
     
   private:
    
    static boost::shared_ptr<crab_os> m_cout;
    static boost::shared_ptr<crab_os> m_cerr;
    
   public:
    
    static boost::shared_ptr<crab_os> cout();
    static boost::shared_ptr<crab_os> cerr();

   private:
    
    std::ostream* m_os;
    
   protected:

    crab_os();

   public:

    crab_os(std::ostream* os);    

    virtual ~crab_os();
    
    virtual crab_os& operator<<(char C);
    virtual crab_os& operator<<(unsigned char C);
    virtual crab_os& operator<<(signed char C);
    virtual crab_os& operator<<(const char *Str);
    virtual crab_os& operator<<(const std::string& Str);
    virtual crab_os& operator<<(unsigned long N);
    virtual crab_os& operator<<(long N);
    virtual crab_os& operator<<(unsigned long long N);
    virtual crab_os& operator<<(long long N);
    virtual crab_os& operator<<(const void *P);
    virtual crab_os& operator<<(unsigned int N);
    virtual crab_os& operator<<(int N);
    virtual crab_os& operator<<(double N);
  };

  extern crab_os& outs();
  extern crab_os& errs();

  // An adaptor for std::ostringstream
  class crab_string_os: public crab_os {

    std::ostringstream* m_string_os;    

   public:

    crab_string_os();

    ~crab_string_os();

    std::string str();

    crab_os& operator<<(char C);
    crab_os& operator<<(unsigned char C);
    crab_os& operator<<(signed char C);
    crab_os& operator<<(const char *Str);
    crab_os& operator<<(const std::string& Str);
    crab_os& operator<<(unsigned long N);
    crab_os& operator<<(long N);
    crab_os& operator<<(unsigned long long N);
    crab_os& operator<<(long long N);
    crab_os& operator<<(const void *P);
    crab_os& operator<<(unsigned int N);
    crab_os& operator<<(int N);
    crab_os& operator<<(double N);    
  };

}// end namespace

template<typename... ArgTypes>
inline void ___print___(ArgTypes... args)
{
  // trick to expand variadic argument pack without recursion
  using expand_variadic_pack = int[];
  // first zero is to prevent empty braced-init-list
  // void() is to prevent overloaded operator, messing things up
  // trick is to use the side effect of list-initializer to call a function
  // on every argument.
  // (void) is to suppress "statement has no effect" warnings
  (void)expand_variadic_pack{0, ((crab::errs() << args), void(), 0)... };
}

#define CRAB_ERROR(...)              \
  do {                               \
    crab::errs() << "CRAB ERROR: ";  \
    ___print___(__VA_ARGS__);        \
    crab::errs() << "\n";            \
    std::exit (EXIT_FAILURE);        \
  } while (0)

#define CRAB_WARN(...)               \
  do {                               \
    crab::errs() << "CRAB WARNING: ";\
    ___print___(__VA_ARGS__);        \
    crab::errs() << "\n";            \
  } while (0)


namespace crab {

   enum variable_type {
     BOOL_TYPE,
     INT_TYPE,
     REAL_TYPE,
     PTR_TYPE,
     ARR_BOOL_TYPE,
     ARR_INT_TYPE,
     ARR_REAL_TYPE,     
     ARR_PTR_TYPE,
     UNK_TYPE};

   inline crab_os& operator<< (crab_os& o, variable_type t)
   {
     switch (t)
     {
       case BOOL_TYPE: o << "bool"; break;       
       case INT_TYPE: o << "int"; break;
       case REAL_TYPE: o << "real"; break;	 
       case PTR_TYPE: o << "ptr"; break;
       case ARR_BOOL_TYPE: o << "arr_bool"; break;	 
       case ARR_INT_TYPE: o << "arr_int"; break;
       case ARR_REAL_TYPE: o << "arr_real"; break;	 
       case ARR_PTR_TYPE: o << "arr_ptr"; break;
       default: o << "unknown"; break;
     }
     return o;
   }
  
   typedef enum { 
     BINOP_ADD, BINOP_SUB, BINOP_MUL, 
     BINOP_SDIV, BINOP_UDIV, BINOP_SREM, BINOP_UREM,
     BINOP_AND, BINOP_OR, BINOP_XOR, BINOP_SHL, BINOP_LSHR, BINOP_ASHR
   } binary_operation_t;

   typedef enum { 
     BINOP_BAND, BINOP_BOR, BINOP_BXOR
   } bool_binary_operation_t;

   typedef enum { 
     CAST_TRUNC, CAST_SEXT, CAST_ZEXT
   } cast_operation_t;
  
   inline crab::crab_os& operator<<(crab::crab_os&o, binary_operation_t op) {
     switch (op) {
       case BINOP_ADD: o << "+"; break;
       case BINOP_SUB: o << "-"; break;
       case BINOP_MUL: o << "*"; break;
       case BINOP_SDIV: o << "/"; break;
       case BINOP_UDIV: o << "/_u"; break;
       case BINOP_SREM: o << "%"; break;
       case BINOP_UREM: o << "%_u"; break;
       case BINOP_AND: o << "&"; break;
       case BINOP_OR: o << "|"; break;
       case BINOP_XOR: o << "^"; break;
       case BINOP_SHL: o << "<<"; break;
       case BINOP_LSHR: o << ">>_l"; break;
       case BINOP_ASHR: o << ">>_r"; break;
       default: CRAB_ERROR("unreachable");
      }
     return o;
   }

   inline crab::crab_os& operator<<(crab::crab_os&o, bool_binary_operation_t op) {
     switch (op) {
       case BINOP_BAND: o << "&"; break;
       case BINOP_BOR: o << "|"; break;
       case BINOP_BXOR: o << "^"; break;
       default: CRAB_ERROR("unreachable");
      }
     return o;
   }

   inline crab::crab_os& operator<<(crab::crab_os&o, cast_operation_t op) {
     switch (op) {
       case CAST_TRUNC: o << "trunc"; break;
       case CAST_SEXT: o << "sext"; break;
       case CAST_ZEXT: o << "zext"; break;
       default: CRAB_ERROR("unreachable");
      }
     return o;
   }
  
  template<typename T>
  inline boost::optional<T> conv_op (binary_operation_t op); 

  template<typename T>
  inline boost::optional<T> conv_op (bool_binary_operation_t op); 

  template<typename T>
  inline boost::optional<T> conv_op (cast_operation_t op); 
  

  // toy language for pointer constraints
  typedef enum  { PTR_EQUALITY, PTR_DISEQUALITY } ptr_cst_kind_t;  

  template<typename Variable>
  class pointer_constraint {
   public:

    typedef pointer_constraint <Variable> ptr_cst_t;
    typedef boost::optional<Variable> opt_var_t;

   private:

    opt_var_t _lhs; 
    opt_var_t _rhs;
    ptr_cst_kind_t _kind;

    /* 
       We can only express constraints of the form p {==,!=} q where
       p,q can be either null or a variable.

       !lhs, !rhs, kind == PTR_EQUALITY    -> null == null -> true
       !lhs, !rhs, kind == PTR_DISEQUALITY -> null != null -> false
       !lhs, rhs, kind == PTR_EQUALITY     -> *rhs == null
       lhs, !rhs, kind == PTR_EQUALITY     -> *lhs == null
       !lhs, rhs, kind == PTR_DISEQUALITY  -> *rhs != null
       lhs, !rhs, kind == PTR_DISEQUALITY  -> *lhs != nul
       lhs, rhs, kind == PTR_EQUALITY      -> *lhs == *rhs
       lhs, rhs, kind == PTR_DISEQUALITY   -> *lhs != *rhs
    */

    pointer_constraint (opt_var_t lhs, opt_var_t rhs, ptr_cst_kind_t kind):
        _lhs (lhs), _rhs (rhs), _kind (kind) {
      if (!_lhs && _rhs){ // normalize
        std::swap (_lhs,_rhs);
      }
    }

   public:

    pointer_constraint () : _kind (PTR_EQUALITY) { }
    
    // return true iff null != null
    bool is_contradiction () const {
      return (!_lhs && !_rhs && _kind == PTR_DISEQUALITY);
    }

    // return true iff null == null
    bool is_tautology () const {
      return (!_lhs && !_rhs && _kind == PTR_EQUALITY);
    }

    bool is_equality () const {
      return (_kind == PTR_EQUALITY);
    }

    bool is_disequality () const {
      return (_kind == PTR_DISEQUALITY);
    }

    // return true iff  p == null or p != null
    bool is_unary () const {
      return (_lhs && !_rhs);
    }

    // return true iff p == q or p != q
    bool is_binary () const {
      return (_lhs && _rhs);
    }

    Variable lhs () const {
      if (!_lhs) CRAB_ERROR ("pointer constraint lhs is null");
      return *_lhs;
    }

    Variable rhs () const {
      if (!_rhs) CRAB_ERROR ("pointer constraint rhs is null");
      return *_rhs;
    }

    static ptr_cst_t mk_true () {
      return ptr_cst_t ();
    }

    static ptr_cst_t mk_false () {
      return ptr_cst_t (opt_var_t (), opt_var_t (), PTR_DISEQUALITY);
    }

    static ptr_cst_t mk_eq_null (Variable v)  {
      return ptr_cst_t (opt_var_t (v), opt_var_t (), PTR_EQUALITY);
    }

    static ptr_cst_t mk_diseq_null (Variable v) {
      return ptr_cst_t (opt_var_t (v), opt_var_t (), PTR_DISEQUALITY);
    }

    static ptr_cst_t mk_eq (Variable v1, Variable v2)  {
      return ptr_cst_t (opt_var_t (v1), opt_var_t (v2), PTR_EQUALITY);
    }

    static ptr_cst_t mk_diseq (Variable v1, Variable v2)  {
      return ptr_cst_t (opt_var_t (v1), opt_var_t (v2), PTR_DISEQUALITY);
    }

    void write (crab::crab_os& o) const {
      if (is_contradiction () ) {
        o << "false";
      } else if (is_tautology ()) {
        o << "true";
      } else {
        assert (_lhs);

        o << lhs ();
        
        if (_kind == PTR_EQUALITY) 
          o << " == ";
        else 
          o << " != ";
        
        if (!_rhs)
          o << "NULL";
        else 
          o <<  rhs ();
      }
    }
  };

  inline crab::crab_os& operator<<(crab::crab_os& o, const ptr_cst_kind_t &k) {
    if (k == PTR_EQUALITY ) 
      o << " == ";
    else 
      o << " != ";
    return o;
  }
    
  template<typename Variable>  
  inline crab::crab_os& operator<<(crab::crab_os& o, 
                                   const pointer_constraint<Variable> &cst) {
    cst.write (o);
    return o;
  }

} // end namespace
 
namespace ikos {
  // Numerical type for indexed objects
  typedef uint64_t index_t;

  // Interface for writeable objects
  class writeable {
   public:
    virtual void write(crab::crab_os& o) = 0;
    virtual ~writeable() {}
  }; // class writeable

  inline crab::crab_os& operator<<(crab::crab_os& o, writeable& x) {
    x.write(o);
    return o;
  }

  // Container for typed variables used by the crab abstract domains
  // and linear_constraints.
  template< typename Number, typename VariableName >  
  class variable {    
    // XXX: template parameter Number is required even if the class
    // does not use it.  This allows, e.g., linear_constraint to
    // deduce the kind of Number from constraints like x < y.

   public:

    typedef variable< Number, VariableName > variable_t;
    typedef typename VariableName::index_t index_t;
    typedef unsigned bitwidth_t;
    typedef crab::variable_type type_t;

  private:
    
    VariableName _n;
    type_t _type;
    bitwidth_t _width;
    
   public:

    /** 
     * DO NOT USE this constructor to create a CFG since all CFG
     * statements must be strongly typed.  This constructor is
     * intended to be used only abstract domains to generate temporary
     * variables.
     **/
    explicit variable(const VariableName& n)
      : _n(n), _type(crab::UNK_TYPE), _width(0) { }

   public:
    
    variable(const VariableName& n, type_t type)
      : _n(n), _type(type), _width(0) { }

    variable(const VariableName& n, type_t type, bitwidth_t width)
      : _n(n), _type(type), _width(width) { }
    
    variable(const variable_t& o)
      : _n(o._n), _type(o._type), _width(o._width) { }

    variable(variable_t &&o)
      : _n(std::move(o._n)), _type(std::move(o._type)), _width(std::move(o._width)) { }

    variable_t& operator=(const variable_t &o) {
      if (this != &o) {
        _n = o._n;
	_type = o._type;
	_width = o._width;
      }
      return *this;
    }

    bool is_typed () const { return _type != crab::UNK_TYPE; }

    bool is_array_type() const {
      return is_typed() && _type >= crab::ARR_BOOL_TYPE;
    }

    bool is_int_type() const {
      return _type == crab::INT_TYPE;
    }

    bool is_bool_type() const {
      return _type == crab::BOOL_TYPE;
    }

    bool is_ptr_type() const {
      return _type == crab::PTR_TYPE;
    }

    type_t get_type() const { return _type;}

    bool has_bitwidth() const { return _width > 0; }
    
    bitwidth_t get_bitwidth() const { return _width;}

    const VariableName& name() const { return _n; }

    // Cannot be const because from VariableName we might want to
    // access to its variable factory to create e.g., new
    // VariableName's.
    VariableName& name() { return _n;}
    
    index_t index() const { return _n.index(); }

    bool operator==(const variable_t& o) const {
      return _n.index () == o._n.index ();
    }

    bool operator!=(const variable_t& o) const {
      return (!(operator==(o)));
    }
    
    bool operator<(const variable_t& o) const {
      return _n.index () < o._n.index ();
    }
    
    void write(crab::crab_os& o) const {
      o << _n;
    }
        
  }; // class variable

  template< typename Number, typename VariableName >  
  class variable_ref {    
   public:

    typedef variable< Number, VariableName > variable_t;
    typedef typename variable_t::index_t index_t;
    typedef typename variable_t::bitwidth_t bitwidth_t;
    typedef typename variable_t::type_t type_t;
    typedef variable_ref<Number, VariableName> variable_ref_t;

  private:
    
    boost::shared_ptr<variable_t> m_v;
    
   public:

    variable_ref(): m_v(nullptr) {}
      
    variable_ref(variable_t v): m_v(boost::make_shared<variable_t>(v)) { }

    bool is_null() const {
      return !m_v;
    }

    variable_t get() const {
      assert(!is_null());
      return *m_v;
    }
    
    bool is_typed () const {
      assert(!is_null());
      return m_v->is_typed();
    }

    bool is_array_type() const {
      assert(!is_null());      
      return m_v->is_array_type();
    }

    bool is_int_type() const {
      assert(!is_null());      
      return m_v->is_int_type();
    }

    bool is_bool_type() const {
      assert(!is_null());
      return m_v->is_bool_type();
    }

    bool is_ptr_type() const {
      assert(!is_null());
      return m_v->is_ptr_type();
    }

    type_t get_type() const {
      assert(!is_null());            
      return m_v->get_type();
    }
    
    bool has_bitwidth() const {
      assert(!is_null());            
      return m_v->has_bitwidth();
    }
    
    bitwidth_t get_bitwidth() const {
      assert(!is_null());            
      return m_v->get_bitwidth();
    }

    const VariableName& name() const {
      assert(!is_null());            
      return m_v->name();
    }

    VariableName& name() {
      assert(!is_null());            
      return m_v->name();
    }
    
    index_t index() const {
      assert(!is_null());            
      return m_v->index();
    }

    bool operator==(const variable_ref_t& o) const {
      assert(!is_null());            
      return m_v->operator==(o);
    }

    bool operator!=(const variable_ref_t& o) const {
      assert(!is_null());            
      return m_v->operator!=(o);      
    }
    
    bool operator<(const variable_ref_t& o) const {
      assert(!is_null());            
      return m_v->operator<(o);            
    }
    
    void write(crab::crab_os& o) const {
      return m_v->write(o);
    }        
  }; // class variable_ref
  
  template< typename Number, typename VariableName >
  inline index_t hash_value (const variable<Number, VariableName> &v) {
    return v.index ();
  }

  template< typename Number, typename VariableName >
  inline index_t hash_value (const variable_ref<Number, VariableName> &v) {
    return v.index ();
  }
  
  template< typename Number, typename VariableName >
  inline crab::crab_os& operator<<(crab::crab_os& o, const variable<Number, VariableName> &v) {
    v.write(o);
    return o;
  }

  template< typename Number, typename VariableName >
  inline crab::crab_os& operator<<(crab::crab_os& o, const variable_ref<Number, VariableName> &v) {
    v.write(o);
    return o;
  }
  
} // end namespace 

namespace crab {
  namespace domains {
    using namespace ikos;
  }
}

