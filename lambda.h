#ifndef _INC_OQ3A_GENTEMPLDEFS_H_
#define _INC_OQ3A_GENTEMPLDEFS_H_

/****************************************************************************/
/**
* Copyright (C) ib-krajewski.de 2007. All Rights Reserved. Confidential.
*
* Distribution only to people who need to know this information in
* order to do their job.(Need-to-know principle).
* Distribution to persons outside the company, only if these persons
* signed a non-disclosure agreement.
* Electronic transmission, e.g. via electronic mail, must be made in
* encrypted form.
*
* @file /.../lambda.h
* @version /main/....
* @author Marek Krajewski
* @date ....
*
* This file contains definition of simple lambda expr. library for use with
* the STL library functions.
*
****************************************************************************/

#include <iosfwd> // for Shift templates
#include <cstdio>


#define DEBUGi(a) printf(#a "=%d\n", a);
#define DEBUGs(a) printf(#a "=%s\n", a);


namespace kmx
{
   /* ----- Preprocessor Constants and Macros ----- */

   /* ----- Declaration of Constants ----- */

   /* ----- Declaration of Types ----- */

   /* ----- Forward Declaration of Structs, Unions and Classes ----- */

   /* ----- Declaration of Structs and Unions ----- */

   /* ----- Declaration of Classes ----- */

   // tag for our curried functions: OPEN -- not used by now!!!
   struct lambda_expr {};


   //===============================================
   // --- unary op's, working on placeholders:
   // look down to "binary" for explanations

   // _$1 =
   template<class T> struct Assgn : public lambda_expr {
      T val;
      Assgn(T t) : val(t) { }
      void operator()(T& t) { t = val; } // assign to iterator!
   };
   
   // &
   struct Addrs : public lambda_expr {
      Addrs() { }
      template<class T> T* operator()(T& t) { return &t; }
   };
   
   // *
   struct Deref : public lambda_expr {
      Deref() {}
      template<class T> T operator()(T* t) const { return *t; }
   };

   // ++
   template<class T> struct Incr : public lambda_expr {
      T& val;
      Incr(T& t) : val(t) { }

      void operator()(T& t) { t++; } // needed for _$1++
      // OPEN todo: preli! Called in another context,
      // -- needed for globvar()!!!
      template <class S> void operator()(S t) { val++; }      
      void operator()() { val++; }
   };

   // ->*
   template<class V, class O> struct ArrowStar : public lambda_expr {
      V O::* mptr;
      ArrowStar(V O::* m) : mptr(m) { }
      V operator()(O* o) const { return o->*mptr; }
      //template <typename Arg>
      // V operator()(O* o, Arg& a) { return o->*mptr(a); }
   };

   template<class R, class O, class A> struct ArrowStarF : public lambda_expr {
      R(O::* fptr)(A);
      ArrowStarF(R(O::* f)(A)) : fptr(f) { }
      R operator()(O* o, A a) const { return o->*fptr(a); }
   };

   template<class R, class O> struct ArrowStarFv : public lambda_expr {
      R(O::* fptr)();
      ArrowStarFv(R(O::* f)()) : fptr(f) { }
      R operator()(O* o) const { return (o->*fptr)(); }
   };

   // placeholders, Perl like:
   // ----
   template <int Int> class placeholder : public lambda_expr
   {
   public:
      template<class T> Assgn<T> operator=(T t) { return Assgn<T>(t); }

      Addrs operator&() { return Addrs(); }
      Deref operator*() { return Deref(); }
      
      template<class V, class O>
      ArrowStar<V, O> operator->*(V O::* m) const { return ArrowStar<V, O>(m); }
      
      template<class R, class O, class A>
      ArrowStarF<R, O, A> operator->*(R O::* f(A)) const { return ArrowStarF<R, O, A>(f); }
      
      template<class R, class O>
      ArrowStarFv<R, O> operator->*(R(O::* f)()) const { return ArrowStarFv<R, O>(f); }
   };

   placeholder<1> _$1;
   placeholder<2> _$2;
   placeholder<3> _$3;
   placeholder<4> _$4;
   

   //===============================================
   // --- binary op's, invoking 1 placeholder

   // Bjarne's idea:
   // You don't have to write function objects,
   // libraries can write them for you (generative programming)!
   // struct Le {}; represents the need to compare using <=

   // <=
   template<class T> struct Le : public lambda_expr {
      T val;
      Le(T t) : val(t) { }
      bool operator()(T t) const { return t <= val; }
   };

   template<class T>
   Le<T> operator<=(T t, placeholder<1>) { return Le<T>(t); }
   
   template<class T>
   Le<T> operator<=(placeholder<1>, T t) { return Le<T>(t); }
   
   // ==
   template<class T> struct Eq : public lambda_expr {
      T val;
      Eq(T t) : val(t) { }
      bool operator()(T t) const { return val == t; }
   };

   template<class T>
   Eq<T> operator==(T t, placeholder<1>) { return Eq<T>(t); }
   
   template<class T>
   Eq<T> operator==(placeholder<1>, T t) { return Eq<T>(t); }

   // *
   template<class T> struct Mul : public lambda_expr {
      T val;
      Mul(T t) : val(t) { }
      T operator()(T t) const { return val * t; }
   };

   template<class T>
   Mul<T> operator*(T t, placeholder<1>) { return Mul<T>(t); }
   
   template<class T>
   Mul<T> operator*(placeholder<1>, T t) { return Mul<T>(t); }
   
   // +=
   template<class T> struct AddAssg : public lambda_expr {
      T& val;
      T val2;
      AddAssg(T& t) : val(t) { }
      AddAssg(T& t, T r) : val(t), val2(r) { }
      
      T operator()(T& t) const { return val += t; }
      T operator()() { return val += val2; } // bound!!

      // OPEN todo: preli! Bound and called in another context,
      // -- needed for globvar()!!!
      template <class S> void operator()(S t) { val += val2; } // bound
   };

   // OPEN, TODO: template<class T> AddAssg<T> operator+=(placeholder<1>, T t) { return AddAssg<T>(t); }

   // =
   template <class T> T& identity(T& t) { return t; }
   // OPEN, TODO: template<class T, class S> and use eval_expr() and
   // return_of() heplers to impl. assignment from lambda_expressions!!!
   
   template<class T> struct AssgnTo : public lambda_expr {
      T& val;
      T val2;
      AssgnTo(T& t, T r) : val(t), val2(r) { }

      void operator()(T& t) { val = t; } // assign to left
      // OPEN todo: preli! Bound and called in another context,
      // -- needed for globvar()!!!
      
      template <class S> void operator()(S t) { val = val2; } // bound

      //S getval;
      //AssgnTo(T &t) : val(t), getval(identity<T>) { }
      //AssgnTo(T &t, S& s) : val(t), getval(s) { }
   };

   // deref ==
   template<class T> struct EqDeref : public lambda_expr {
      T val;
      EqDeref(T t) : val(t) { }
      bool operator()(T* t) const { return val == *t; }
   };

   template<class T>
   EqDeref<T> operator==(T t, Deref) { return EqDeref<T>(t); }
   
   template<class T>
   EqDeref<T> operator==(Deref, T t) { return EqDeref<T>(t); }

   // lambda_expr ==
   template<class S, class T> struct EqTo : public lambda_expr {
      S lexpr;
      T val;
      EqTo(S s, T t) : lexpr(s), val(t) { }

      template <class R>
      bool operator()(R r) { return lexpr(r) == val; }

      // OPEN todo: preli! Bound and called in another context,
      // -- needed for globvar()!!!
      //template <class S> void operator()(S t) { val = val2; } // bound
   };

   template <class S, class T> EqTo<S, T> operator==(S s, T t) { return EqTo<S, T>(s, t); }

   // lambda_expr !=
   //  - mrkkrj: added 2021
   template<class S, class T> struct NEqTo : public lambda_expr {
      S lexpr;
      T val;
      NEqTo(S s, T t) : lexpr(s), val(t) { }

      template <class R>
      bool operator()(R r) { return lexpr(r) != val; }

      // OPEN todo: preli! Bound and called in another context,
      // -- needed for globvar()!!!
      //template <class S> void operator()(S t) { val != val2; } // bound
   };

   template <class S, class T> NEqTo<S, T> operator!=(S s, T t) { return NEqTo<S, T>(s, t); }


   //===============================================
   // ---- binary, for 2 placeholders / lambda expr.
   
   // helpers ??? needed:
   template <class T, class S> S return_of(T& exp, S& val) { return val; }
   template <class S> S return_of(const Deref& exp, S*& val) { return *val; }
   template <class S, class O> S O::* return_of(const ArrowStar<O, S>& exp, O* obj) { return 0; }
   
   // helpers contd.
   template <class S, class T, class U> U eval_expr(const S& exp, T val, U ret) { return exp(val); }

   // a <= b
   struct Le2 : public lambda_expr
   {
      Le2() { }
      template<class T> bool operator()(T a, T b) const { return a <= b; }
   };

   template <class S, class T> struct Le2_forw : public lambda_expr
   {
      S e1;
      T e2;
      Le2_forw(S s, T t) : e1(s), e2(t) { }

      template <class U>
      // OPEN TODO: change it in other contexts!!!
      bool operator()(U a, U b) const { return e1(a) <= e2(a); }
      
      // OR needed??? ... <= eval_expr(e2,b, return_of(e2, b)); }
      
      template <class U> // one side is bound! OPEN: assume right side!!!
      bool operator()(U a) const { return e1(a) <= e2; }
   };

   Le2 operator<=(placeholder<1>, placeholder<2>) { return Le2(); }

   template <class S, class T> // only f r sort,
   Le2_forw<S, T> operator<=(S s, T t) { return Le2_forw<S, T>(s, t); }

   // a >= b
   struct Ge2 : public lambda_expr
   {
      Ge2() { }
      template<class T> bool operator()(T a, T b) const { return a >= b; }
   };

   template <class S, class T> struct Ge2_forw : public lambda_expr
   {
      S e1;
      T e2;
      Ge2_forw(S s, T t) : e1(s), e2(t) { }
      template <class U>
      bool operator()(U a, U b) const { return e1(a) >= e2(b); }
   };

   Ge2 operator>=(placeholder<1>, placeholder<2>) { return Ge2(); }

   template <class S, class T> // for 2 lambda expr
   Ge2_forw<S, T> operator>=(S s, T t) { return Ge2_forw<S, T>(s, t); }


   //=============================================
   // ---- delay evaluation of const and vars
   //=============================================

   // constant_ref
   // ---
   template<class T> struct Const : public lambda_expr
   {
      T& val;
      Const(T& t) : val(t) { }

      const T& operator()() const { return val; }
      const T& value() const { return val; }
      
      template <class S> // for different context: needed for Shift!!!
      const T& operator()(S& s) const { return val; } // ignore input
   };

   template<class T>
   Const<T> delay(T& t) { return Const<T>(t); }

   // variable_ref
   // ---
   template<class T> struct Var : public lambda_expr
   {
      T& val;
      Var(T& t) : val(t) { }

      T& operator()() const { return val; }
      T& operator()(T& t) const { return val; } // ignore input
      T& value() const { return val; }
      
      AssgnTo<T> operator=(T t) { return AssgnTo<T>(val, t); }
      
      template <class S> // assign from lambda_expr
      AssgnTo<T> operator=(S s) { return AssgnTo<T>(val, s); }
   };

   template<class T> Var<T> globvar(T& t) { return Var<T>(t); }

   // OPEN todo: be more modular!!!
   // --- return lambda_operation<lambda_exp, oper_type>
   template<class T>
   AddAssg<T> operator+=(Var<T> v, const T& t) { return AddAssg<T>(v.value(), t); }
   
   template<class T>
   AddAssg<T> operator+=(Var<T> v, placeholder<1>) { return AddAssg<T>(v.value()); }
   
   template<class T>
   Incr<T> operator++(Var<T> v, int) { return Incr<T>(v.value()); }
   
   int dummy_int; // OPEN TODO: preliminary...
   Incr<int> operator++(placeholder<1>, int) { return Incr<int>(dummy_int); }


   //===============================================
   // -- binding: currying
   //===============================================

   // typelists
   template <class A1> class list1
   {
      A1 m_a1;
   public:
      explicit list1(A1 a1) : m_a1(a1) {}

      // if placeholder:
      A1 operator[] (placeholder<1>) const { return m_a1; }
      
      // if lamba expr: apply
      //A1 operator[] (lambda_expr& e) const { return e(); }
      // --> OPEN, TODO: expand to general case!!!
      template <class T>
      T operator[] (Mul<T>& e) const { return e(m_a1); }
      
      // not placeholders: return the ball
      template <class T>
      T operator[] (T val) const { return val; }
      
      // apply to arguments and placeholders!
      template <class F, class List1>
      void operator() (F f, List1 list1)
      {
         f(list1[m_a1]);
      }
      
      // bind a void member function e.g.: int (A::*)(void *)
      template <class T, typename Ret, class List1>
      Ret operator() (Ret(T::* f)(void), List1 list1)
      {
         return ((list1[m_a1])->*f)();
      }
   };

   template <class A1, class A2> class list2
   {
      // args: one of them is a placeholder!
      // -- list1 will detect it!
      A1 m_a1;
      A2 m_a2;

   public:
      list2(A1 a1, A2 a2) : m_a1(a1), m_a2(a2) {};

      // apply to arguments and placeholders!
      template <class F, class List1>
      void operator() (F f, List1 list1)
      {
         f(list1[m_a1], list1[m_a2]);
      }
   };

   template <class A1, class A2, class A3> class list3
   {
      // args: one of them is a placeholder!
      // -- list1 will detect it!
      A1 m_a1;
      A2 m_a2;
      A3 m_a3;

   public:
      list3(A1 a1, A2 a2, A3 a3)
         : m_a1(a1), m_a2(a2), m_a3(a3) {}

      // apply to arguments and placeholders!
      template <class F, class List1>
      void operator() (F f, List1 list1)
      {
         f(list1[m_a1], list1[m_a2], list1[m_a3]);
      }
   };

   // binder class for currying:
   // ---
   template <class F, class List> class binder : public lambda_expr
   {
      F m_f;
      List m_list;

   public:
      binder(F f, List list) : m_f(f), m_list(list) {}

      // OPEN TODO --> only working for void funct!!!
      template <class A1> void operator() (A1 a1)
      {
         list1<A1> list(a1); // store the actual arg
         m_list(m_f, list); // forward to curried funct: m_f+m_list
      }
   };

   // bind functions:
   // ---

   // 3 args
   template <class F, class A1, class A2, class A3>
   binder<F, list3<A1, A2, A3> > bind(F f, A1 a1, A2 a2, A3 a3)
   {
      typedef list3<A1, A2, A3> list3_type;
      list3_type list(a1, a2, a3);
      return binder<F, list3_type>(f, list);
   }

   // 2 args
   template <class F, class A1, class A2>
   binder<F, list2<A1, A2> > bind(F f, A1 a1, A2 a2)
   {
      typedef list2<A1, A2> list2_type;
      list2_type list(a1, a2);
      return binder<F, list2_type>(f, list);
   }

   // 1 arg
   template <class F, class A1>
   binder<F, list1<A1> > bind(F f, A1 a1)
   {
      typedef list1<A1> list1_type;
      list1_type list(a1);
      return binder<F, list1_type>(f, list);
   }


   //===============================================
   // --- ostream << parse:
   //===============================================

   // -- simple solution: only cout << _$1 !!!
#if 0
   template<class S> struct Shift : public lambda_expr {
      S& target;
      Shift(S& t) : target(t) { }
      template <class T>
      S& operator()(T& t) const { return target << t; }
   };

   template<class S>
   Shift<S> operator<< (S& s, placeholder<1>) { return Shift<S>(s); }
   template<class S, class T>
   Shift<S> operator<< (S& s, ) { return Shift<S>(s); }
#endif

   // general solution
   // --- Expression Templates :-O for << a << b << c...
   struct shiftOp // Represents << operation
   {
      template <class Left, class Right, class T>
      static void print(Left& left, Right& right, T& t)
      {
         print(left.leftNode, left.rightNode, t); // walk down
         left.out << right(t); // OPEN -- if right needs t? : << _$1*2 ???
      }

      template <class Right, class T>
      static void print(std::ostream& left, Right& right, T& t)
      {
         left << right(t); // at the beginning
      }

      template <class Left, class T>
      static void print(Left& left, placeholder<1>& right, T& t)
      {
         print(left.leftNode, left.rightNode, t);
         left.out << t; // special case placeholder
      }
   };

   // Shift represents a node in the parse tree
   // ---
   template<typename Left, typename Op, typename Right>
   struct Shift : public lambda_expr
   {
      Left leftNode;
      Right rightNode;
      std::ostream& out;

      Shift(Left t1, Right t2, std::ostream& os)
         : leftNode(t1), rightNode(t2), out(os) { }

      template <class T>
      void operator() (T& t) { Op::print(leftNode, rightNode, t); }
   };

   // The overloaded operator which does parsing for expressions of the
   // form "a<<b<<c<<d" => Shift<Shift<Shift<A, op, B>, op, C>, op, D>()
   // for ostream: cannot be taken by value!
   template<class Left, class Right>
   Shift<Left&, shiftOp, Right> operator<< (Left& left, Right right) {
      return Shift<Left&, shiftOp, Right>(left, right, left);
      // left IS an ostream!
   }

   // for lambda_expr: must be taken by value!
   template<class Left1, class Left2, class Right>
   Shift<Shift<Left1, shiftOp, Left2>, shiftOp, Right>
      operator<< (Shift<Left1, shiftOp, Left2> left, Right right) {
      return Shift<Shift<Left1, shiftOp, Left2>, shiftOp, Right>(left, right, left.out);
   }

   // TODO --> later... use superclass lambda_expr?
   //Shift<def_ostream&, shiftOp, lambda_expr> operator<< (def_ostream& left, lambda_expr right) { return Shift<def_ostream&, shiftOp, lambda_expr>(left, right); }
   //#endif


   //=============================================
   // ---- control structures:
   //=============================================

   // if_then
   // ---
   // helper: Assgn needs the iterator for: if_then(_$1==0, _$1=44)
   // --- OPEN, TODO: make general for e.g.: if_then(_$1>=3, cout << _$1)
   // --
   template <class S, class T> void eval_then_expr(S& e, T& t) { e(); }
   template <class T> void eval_then_expr(Assgn<T>& e, T& t) { e(t); }
   template<class S, class T> struct IfThen : public lambda_expr {
      S if_expr;
      T then_expr;

      IfThen(S s, T t) : if_expr(s), then_expr(t) {}
      
      template <class U>
      // OPEN, TODO: check if then_expr needs the val argument!
      bool operator()(U& val) {
         // correction 2021!!!
         // OPEN:: how did this even compile back then????
#if 0    
         if (if_expr(val))
            eval_then_expr(then_expr, val);
#else
         if (if_expr(val)) {
            eval_then_expr(then_expr, val);
            return true;
         }
         else {
            return false;
         }
#endif
         // correction 2021 - END...
      }
   };

   template <class S, class T>
   IfThen<S, T> if_then(S s, T t) { return IfThen<S, T>(s, t); }

   // t.b.c.....

}

#endif
