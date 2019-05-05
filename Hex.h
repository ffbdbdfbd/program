#if !defined(AFX_HEX_H__B1CDB7E5_AB5E_4C84_B526_CF235404C620__INCLUDED_)
#define AFX_HEX_H__B1CDB7E5_AB5E_4C84_B526_CF235404C620__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000

class Hex{
public:
	// шестнадцатеричный формат
	virtual void TextToHex(char *data, int length) = 0;
};

#endif // !defined(AFX_HEX_H__B1CDB7E5_AB5E_4C84_B526_CF235404C620__INCLUDED_)
