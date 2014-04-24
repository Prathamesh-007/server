// TABUTIL.H     Olivier Bertrand    2013
// Defines the TAB catalog tables

#ifndef TABUTIL
#define TABUTIL 1

//#include "tabtbl.h"

typedef class PRXDEF *PPRXDEF;
typedef class TDBPRX *PTDBPRX;
typedef class XXLCOL *PXXLCOL;
typedef class PRXCOL *PPRXCOL;
typedef class TBCDEF *PTBCDEF;
typedef class TDBTBC *PTDBTBC;

TABLE_SHARE *GetTableShare(PGLOBAL g, THD *thd, const char *db, 
                                      const char *name, bool& mysql);
PQRYRES TabColumns(PGLOBAL g, THD *thd, const char *db, 
                                        const char *name, bool& info);

void Remove_tshp(PCATLG cat);

/* -------------------------- PROXY classes -------------------------- */

/***********************************************************************/
/*  PROXY: table based on another table. Can be used to have a         */
/*  different view on an existing table.                               */
/*  However, its real use is to be the base of TBL and PRX tables.     */
/***********************************************************************/

/***********************************************************************/
/*  PRX table.                                                         */
/***********************************************************************/
class DllExport PRXDEF : public TABDEF {  /* Logical table description */
	friend class TDBPRX;
	friend class TDBTBC;
 public:
  // Constructor
	 PRXDEF(void);

  // Implementation
  virtual const char *GetType(void) {return "PRX";}

  // Methods
  virtual bool DefineAM(PGLOBAL g, LPCSTR am, int poff);
  virtual PTDB GetTable(PGLOBAL g, MODE mode);

 protected:
  // Members
  PTABLE  Tablep;                                /* The object table   */
  }; // end of PRXDEF

/***********************************************************************/
/*  This is the class declaration for the XCSV table.                  */
/***********************************************************************/
class DllExport TDBPRX : public TDBASE {
  friend class PRXDEF;
  friend class PRXCOL;
 public:
  // Constructor
  TDBPRX(PPRXDEF tdp);

  // Implementation
  virtual AMT   GetAmType(void) {return TYPE_AM_PRX;}

  // Methods
  virtual int   GetRecpos(void) {return Tdbp->GetRecpos();}
	virtual void  ResetDB(void) {Tdbp->ResetDB();}
	virtual int   RowNumber(PGLOBAL g, bool b = FALSE);
  virtual PSZ   GetServer(void) {return (Tdbp) ? Tdbp->GetServer() : (PSZ)"?";}

  // Database routines
	virtual PCOL  MakeCol(PGLOBAL g, PCOLDEF cdp, PCOL cprec, int n);
  virtual bool  InitTable(PGLOBAL g);
  virtual int   GetMaxSize(PGLOBAL g);
  virtual bool  OpenDB(PGLOBAL g);
  virtual int   ReadDB(PGLOBAL g);
  virtual int   WriteDB(PGLOBAL g);
  virtual int   DeleteDB(PGLOBAL g, int irc);
  virtual void  CloseDB(PGLOBAL g) {if (Tdbp) Tdbp->CloseDB(g);}
        PTDBASE GetSubTable(PGLOBAL g, PTABLE tabp, bool b = false);
          void  RemoveNext(PTABLE tp);

 protected:
  // Members
  PTDBASE Tdbp;                   // The object table
  }; // end of class TDBPRX

/***********************************************************************/
/*  Class PRXCOL: PRX access method column descriptor.                 */
/*  This A.M. is used for PRX tables.                                  */
/***********************************************************************/
class DllExport PRXCOL : public COLBLK {
  friend class TDBPRX;
  friend class TDBTBL;
  friend class TDBOCCUR;
 public:
  // Constructors
  PRXCOL(PCOLDEF cdp, PTDB tdbp, PCOL cprec, int i, PSZ am = "PRX");

  // Implementation
  virtual int    GetAmType(void) {return TYPE_AM_PRX;}

  // Methods
  virtual void   Reset(void);
  virtual bool   IsSpecial(void) {return Pseudo;}
  virtual void   ReadColumn(PGLOBAL g);
          bool   Init(PGLOBAL g);

 protected:
  // Default constructor not to be used
  PRXCOL(void) {}

  // Members
  PCOL     Colp;               // Points to matching table column
  PVAL     To_Val;             // To the matching column value
  bool     Pseudo;             // TRUE for special columns
  int      Colnum;             // Used when retrieving columns by number
  }; // end of class PRXCOL

/***********************************************************************/
/*  This is the class declaration for the TBC column catalog table.    */
/***********************************************************************/
class TDBTBC : public TDBCAT {
 public:
  // Constructors
  TDBTBC(PPRXDEF tdp);

 protected:
	// Specific routines
	virtual PQRYRES GetResult(PGLOBAL g);

  // Members
  PSZ     Db;                    // Database of the table  
  PSZ     Tab;                   // Table name            
  }; // end of class TDBMCL

#endif // TABUTIL
