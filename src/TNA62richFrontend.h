#ifndef NA62_RICH_FRONTEND_H
#define NA62_RICH_FRONTEND_H

#define  MAXCH  2048  // total number of electronic channels
#define  MAXPIX 1952  // total number of pmt

bool IsConnected       (int echannel);

int  GetDisk           (int echannel);
int  GetSector         (int echannel);
int  GetFeBoard        (int echannel);
int  GetFeBoardAbsolute(int echannel);
int  GetFeChannel      (int echannel);

int  GetAsic           (int ch);
int  GetAsicAbsolute   (int ch);
int  GetAsicChannel    (int ch);

#endif
