/* 
   ==================================================================
   Photon map support routines for scattering by materials. 
   
   Roland Schregle (roland.schregle@{hslu.ch, gmail.com})
   (c) Fraunhofer Institute for Solar Energy Systems,
   (c) Lucerne University of Applied Sciences and Arts,
   supported by the Swiss National Science Foundation (SNSF, #147053)
   ==================================================================
   
   $Id: pmapmat.h,v 2.3 2015/05/21 13:54:59 greg Exp $
*/


#ifndef PMAPMAT_H
   #define PMAPMAT_H

   #include "pmap.h"

   /* 
      Checks for paths already accounted for in photon map to avoid
      double-counting during backward raytracing.
    
      ambRayInPmap():      Check for DIFFUSE -> (DIFFUSE|SPECULAR) -> *
                           subpaths.  These occur during the backward pass
                           when an ambient ray spawns an indirect diffuse or
                           specular ray.  These subpaths are already
                           accounted for in the global photon map.
      
      shadowRayInPmap():   Check for DIFFUSE -> SPECULAR -> LIGHT
                           subpaths. These occur during the backward pass 
                           when a shadow ray is transferred through a
                           transparent material. These subpaths are already
                           accounted for by caustic photons in the global,
                           contrib, or dedicated caustic photon map.
   */
   #define ambRayInPmap(r)    ((r) -> crtype & AMBIENT && \
                               ((photonMapping && \
                                 (ambounce < 0 || (r) -> rlvl > 1)) || \
                                 causticPhotonMapping || contribPhotonMapping))
   #define shadowRayInPmap(r) ((r) -> crtype & SHADOW && \
				photonMapping)
   
   /* Check if scattered ray spawns a caustic photon */
   #define PMAP_CAUSTICRAY(r) ((r) -> rtype & SPECULAR)


   /* Scattered photon ray types for photonRay() */
   #define  PMAP_DIFFREFL        (REFLECTED | AMBIENT)
   #define  PMAP_DIFFTRANS       (REFLECTED | AMBIENT | TRANS)
   #define  PMAP_SPECREFL        (REFLECTED | SPECULAR)
   #define  PMAP_SPECTRANS       (REFLECTED | SPECULAR | TRANS)
   #define  PMAP_REFRACT         (REFRACTED | SPECULAR)
   #define  PMAP_XFER            (TRANS)



   /* Dispatch table for photon scattering functions */
   extern int (*photonScatter []) (OBJREC*, RAY*);
   
   /* List of antimatter sensor modifier names */
   extern char *photonSensorList [MAXSET + 1];



   /* Spawn a new photon ray from a previous one; this is effectively a
    * customised rayorigin(). */
   void photonRay (const RAY *rayIn, RAY *rayOut, int rayOutType, 
                   COLOR fluxAtten);

   /* Init photonScatter[] dispatch table with material specific scattering
      routines */
   void initPhotonScatterFuncs ();

   /* Find antimatter geometry declared as photon sensors */   
   void getPhotonSensors (char **sensorList);
   
#endif
