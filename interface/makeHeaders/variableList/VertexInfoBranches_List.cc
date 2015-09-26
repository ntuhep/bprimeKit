Int_t     Size;
Int_t     isValid        [MAX_Vertices] ;
Bool_t    isFake         [MAX_Vertices] ; //Uly 2011-04-04
Int_t     Type           [MAX_Vertices] ;   //0 - Offline Primary Vertices, 1 - Offline Primary Vertices with beam spot constraint, 2 - Pixel Vertices
Float_t   Ndof           [MAX_Vertices] ;
Float_t   NormalizedChi2 [MAX_Vertices] ;
Float_t   Pt_Sum         [MAX_Vertices] ;
Float_t   Pt_Sum2        [MAX_Vertices] ;
Float_t   x              [MAX_Vertices] ;
Float_t   y              [MAX_Vertices] ;
Float_t   z              [MAX_Vertices] ;
Float_t   Rho            [MAX_Vertices] ;
