import numpy as np


def solve_matrix_equation(A, B):
    """
    Solve the matrix equation AX = B for X.

    Parameters
    ----------
    A : array_like
        Coefficient matrix.
    B : array_like
        Right-hand side matrix.

    Returns
    -------
    X : array_like
        Solution to the matrix equation AX = B.

    """
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


if __name__ == "__main__":
    example_usage()
