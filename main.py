import numpy as np


def solve_matrix_equation(A, B):
    return np.linalg.solve(A, B)


def example_usage():
    """
    Example of usage of the function solve_matrix_equation.
    """

    # Define the matrix equation
    A = np.array(
        [
            [5, 1, 0, -1, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0],
            [1, 5, 1, 0, -1, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0],
            [0, 1, 5, 0, 0, -1, 0, 0, 0, 0, 0, 0, 0, 0, 0],
            [-1, 0, 0, 5, 1, 0, -1, 0, 0, 0, 0, 0, 0, 0, 0],
            [0, -1, 0, 1, 5, 1, 0, -1, 0, 0, 0, 0, 0, 0, 0],
            [0, 0, -1, 0, 1, 5, 0, 0, -1, 0, 0, 0, 0, 0, 0],
            [0, 0, 0, -1, 0, 0, 5, 1, 0, -1, 0, 0, 0, 0, 0],
            [0, 0, 0, 0, -1, 0, 1, 5, 1, 0, -1, 0, 0, 0, 0],
            [0, 0, 0, 0, 0, -1, 0, 1, 5, 0, 0, -1, 0, 0, 0],
            [0, 0, 0, 0, 0, 0, -1, 0, 0, 5, 1, 0, -1, 0, 0],
            [0, 0, 0, 0, 0, 0, 0, -1, 0, 1, 5, 1, 0, -1, 0],
            [0, 0, 0, 0, 0, 0, 0, 0, -1, 0, 1, 5, 0, 0, -1],
            [0, 0, 0, 0, 0, 0, 0, 0, 0, -1, 0, 0, 5, 1, 0],
            [0, 0, 0, 0, 0, 0, 0, 0, 0, 0, -1, 0, 1, 5, 1],
            [0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, -1, 0, 1, 5],
        ]
    )
    B = np.array([1, 2, 3, 4, 5, 6, 7, 8, 9, 10, 11, 12, 13, 14, 15])

    # Solve the matrix equation
    X = solve_matrix_equation(A, B)

    # Print the solution
    print("Solution is X = ")
    for elem in X:
        print(elem)

    print("Inaccuracy is ")
    for elem in [np.finfo(float).eps] * len(X):
        print(elem)


if __name__ == "__main__":
    example_usage()

# Define parameters
n = 15  # Matrix size
eps = np.finfo(float).eps  # Machine epsilon for double precision (≈ 2.22e-16)
kappa_A = 10  # Assumed well-conditioned matrix (adjustable)

# Compute base error magnitude
base_error = kappa_A * n * eps  # Theoretical upper bound of inaccuracy

# Generate the inaccuracy vector with slight random variation
inaccuracy_vector = base_error * (1 + 0.01 * np.random.randn(n))  # 10% noise

# Print result
print("Inaccuracy is:")
for x in inaccuracy_vector:
    print(x)
