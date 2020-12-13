_ = {
   camera = {
     fov = 60, -- for radians, use fov_radians
     -- aspect = 1,
     near = 1,
     far = 1000,
     transform = {
        position = {0, 0, 5}
     },
     look_at = {
        at = {0, 0, 0},
        up = {0, 1, 0}
     }
   },
   objects = {
      {
         mesh = "cube"
      },
      {
         mesh = "sphere",
         transform = {
            position = {1, 1, 1}
         }
      },
      {
         mesh = "models/african_head.obj",
         material = {
            diffuse = "models/african_head_diffuse.tga",
         },
         transform = {
            position = {0, 0, 0},
            rotation = {0, 0, 0}
         }
      }
   }
}
