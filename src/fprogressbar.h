// File: fprogressbar.h
// Provides: class FProgressbar
//
//  Inheritance diagram
//  ═══════════════════
//
// ▕▔▔▔▔▔▔▔▔▔▏ ▕▔▔▔▔▔▔▔▔▔▏
// ▕ FObject ▏ ▕  FTerm  ▏
// ▕▁▁▁▁▁▁▁▁▁▏ ▕▁▁▁▁▁▁▁▁▁▏
//      ▲           ▲
//      │           │
//      └─────┬─────┘
//            │
//       ▕▔▔▔▔▔▔▔▔▏
//       ▕ FVTerm ▏
//       ▕▁▁▁▁▁▁▁▁▏
//            ▲
//            │
//       ▕▔▔▔▔▔▔▔▔▔▏
//       ▕ FWidget ▏
//       ▕▁▁▁▁▁▁▁▁▁▏
//            ▲
//            │
//    ▕▔▔▔▔▔▔▔▔▔▔▔▔▔▔▏
//    ▕ FProgressbar ▏
//    ▕▁▁▁▁▁▁▁▁▁▁▁▁▁▁▏

#ifndef FPROGRESSBAR_H
#define FPROGRESSBAR_H

#include "fwidget.h"


//----------------------------------------------------------------------
// class FProgressbar
//----------------------------------------------------------------------

#pragma pack(push)
#pragma pack(1)

class FProgressbar : public FWidget
{
  public:
    // Using-declarations
    using FWidget::setGeometry;

    // Constructor
    explicit FProgressbar(FWidget* = 0);

    // Destructor
    virtual ~FProgressbar();

    // Accessors
    const char*  getClassName() const;
    int          getPercentage();

    // Mutators
    void         setPercentage (int);
    void         setGeometry (int, int, int, int, bool = true);
    bool         setShadow (bool);
    bool         setShadow();
    bool         unsetShadow();

    // Inquiries
    bool         hasShadow();

    // Methods
    void         hide();
    void         reset();

  private:
    // Methods
    virtual void draw();
    void         drawPercentage();
    void         drawBar();

    // Data Members
    int          percentage;
    int          bar_length;
};
#pragma pack(pop)


// FProgressbar inline functions
//----------------------------------------------------------------------
inline const char* FProgressbar::getClassName() const
{ return "FProgressbar"; }

//----------------------------------------------------------------------
inline int FProgressbar::getPercentage()
{ return percentage; }

//----------------------------------------------------------------------
inline bool FProgressbar::setShadow()
{ return setShadow(true); }

//----------------------------------------------------------------------
inline bool FProgressbar::unsetShadow()
{ return setShadow(false); }

//----------------------------------------------------------------------
inline bool FProgressbar::hasShadow()
{ return ((flags & fc::shadow) != 0); }


#endif  // FPROGRESSBAR_H
