### Learn and Practice modern OpenGL
  
A step by step flow for learning modern OpenGL.  
**Following the guide and layout by Joey De Vries: https://learnopengl.com/**

Summary notes provided at the end of each program, for a brief overview.  
  
1. A solid colored window  

2. Triangles
    - Solid colored triangle
    - Solid rectangle using the above code
    - Solid color, changing with time (Use of uniforms)
    - Interpolated color fill in triangle    

 3. Shader Class (to create shaders)  

 4. Use of multiple VAO, VBO
    - Use multiple VAO to draw two triangles
    - Use multiple VAO, and multiple shaders to draw two triangles (one with uniform, one with interpolation)  

 5. Use of textures
    - Application of a texture to a triangle  
    - Application of a texture to a rectangle
    - Apply a mix of two textures
    - Animate the interpolation of two textures

 6. Transformations and Coordinate Systems
    - Use of matrices for transformations (Rotate and scale the plane) 
    - Various coordinate systems involved
    - A textured cube (Move from plane to cube)
    - Use of Z-buffer to get depth accurate results
    - Display multiple cubes by changing the model matrix
    - Display interpolated color on cube faces

 7. Camera 
    - Simple linear camera motion (via view matrix)
    - Creating a basic camera class (does the same as above)
    - Have option to move camera using WASD
 
 8. User Interactivity
    - Mouse button callback
    - Cursor callback
    - Key callback

 9. Basic Lighting
    - Object color and a basic light
    - Basic lighting (ambient, diffuse, specular)
