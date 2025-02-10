import numpy as np
import matplotlib.pyplot as plt
from scipy.sparse import diags
from scipy.sparse.linalg import spsolve

class InputParameters:
    def __init__(self, xl, xr, yl, yr, u1, hi2, k1, u2, u3, u4, f):
        self.xl = xl
        self.xr = xr
        self.yl = yl
        self.yr = yr
        self.u1 = u1  # Dirichlet on x = xl
        self.hi2 = hi2  # Coefficient for Robin condition on x = xr
        self.k1 = k1  # Function for diffusion coefficient
        self.u2 = u2  # Robin condition function on x = xr
        self.u3 = u3  # Dirichlet on y = yl
        self.u4 = u4  # Dirichlet on y = yr
        self.f = f  # Source function

def solve_pde(params, nx=50, ny=50):
    dx = (params.xr - params.xl) / (nx - 1)
    dy = (params.yr - params.yl) / (ny - 1)
    
    # Grid setup
    x_vals = np.linspace(params.xl, params.xr, nx)
    y_vals = np.linspace(params.yl, params.yr, ny)
    
    # Number of unknowns
    N = nx * ny
    
    # Coefficient matrix and right-hand side
    A = diags([-1, 4, -1], [-1, 0, 1], shape=(N, N)).tolil()
    b = np.zeros(N)
    
    # Construct the finite difference scheme
    for j in range(1, ny - 1):
        for i in range(1, nx - 1):
            idx = j * nx + i
            A[idx, idx] = -2 * (1/dx**2 + 1/dy**2)
            A[idx, idx - 1] = params.k1(x_vals[i]) / dx**2
            A[idx, idx + 1] = params.k1(x_vals[i]) / dx**2
            A[idx, idx - nx] = 1 / dy**2
            A[idx, idx + nx] = 1 / dy**2
            
            # Apply source function
            b[idx] = params.f(x_vals[i], y_vals[j])
    
    # Apply Dirichlet boundary conditions
    for i in range(nx):
        A[i, i] = 1  # Bottom boundary y = yl
        A[N - nx + i, N - nx + i] = 1  # Top boundary y = yr
        b[i] = params.u3(x_vals[i])
        b[N - nx + i] = params.u4(x_vals[i])
    
    for j in range(ny):
        A[j * nx, j * nx] = 1  # Left boundary x = xl
        b[j * nx] = params.u1(y_vals[j])
    
    # Apply Robin boundary condition at x = xr
    for j in range(ny):
        idx = j * nx + (nx - 1)
        A[idx, idx] = params.hi2 + params.k1(params.xr) / dx
        A[idx, idx - 1] = -params.k1(params.xr) / dx
        b[idx] = params.u2(y_vals[j])
    
    # Convert to sparse format and solve
    A = A.tocsc()
    u = spsolve(A, b)
    
    # Reshape solution into 2D array
    u = u.reshape((ny, nx))

    print(u)
    
    # Plot result
    plt.figure(figsize=(6, 5))
    plt.imshow(u, extent=[params.xl, params.xr, params.yl, params.yr], origin='lower', cmap='jet')
    plt.colorbar(label='u(x, y)')
    plt.title('Numerical Solution of the PDE')
    plt.xlabel('x')
    plt.ylabel('y')
    plt.show()
    
# Example usage
params = InputParameters(
    xl=1.0, xr=10.0, yl=1.0, yr=5.0,
    u1=lambda y: 3 + 2 * y**3,
    hi2=5.0,
    k1=lambda x: 2.0,
    u2=lambda y: 15000 + 10 * y**3 + 1800,
    u3=lambda x: 3 * x**3 + 2,
    u4=lambda x: 3 * x**3 + 250,
    f=lambda x, y: -36 * x - 12 * y
)

solve_pde(params, nx=5, ny=5)
