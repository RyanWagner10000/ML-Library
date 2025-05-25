#include "../header/linear_regression.h"

int predict(Vector *x, Vector *w, double b, double *result)
{
    return dot_product(x, w, result);
}

int mse(Matrix *x, Vector *y, Vector *w, double b, double *result)
{
    double y_pred = 0;
    *result = 0;
    for (int i = 0; i < x->rows; ++i)
    {
        Vector xi = {&x->data[i * x->cols], x->cols};

        if (predict(&xi, w, b, &y_pred) < 0)
        {
            return -1;
        }

        double error = y->data[i] - y_pred;
        *result += error * error;
    }
    *result /= x->rows;
    return 0;
}

int computeGradients(Matrix *x, Vector *y_true, Vector *w, double b, Vector *grad_w, double *grad_b)
{
    for (int j = 0; j < w->size; ++j)
    {
        grad_w->data[j] = 0.0;
    }
    *grad_b = 0.0;

    for (int i = 0; i < x->rows; ++i)
    {
        Vector xi = {&x->data[i * x->cols], x->cols};

        double y_pred = 0;
        if (predict(&xi, w, b, &y_pred) < 0)
        {
            return -1;
        }
        double error = y_true->data[i] - y_pred;

        for (int j = 0; j < w->size; ++j)
        {
            grad_w->data[j] += -2 * error * xi.data[j];
        }

        *grad_b += -2 * error;
    }

    for (int j = 0; j < w->size; ++j)
    {
        grad_w->data[j] /= x->rows;
    }

    *grad_b /= x->rows;

    return 0;
}

int train_linear_model(Matrix *x, Vector *y_true, Vector *w, double *b, double lr, int epochs)
{
    Vector grad_w = {malloc(sizeof(double) * w->size), w->size};
    double grad_b = 0.0;

    for (int epoch = 0; epoch < epochs; ++epoch)
    {
        if (computeGradients(x, y_true, w, *b, &grad_w, &grad_b) < 0)
        {
            return -1;
        }

        for (int j = 0; j < w->size; ++j)
        {
            w->data[j] -= lr * grad_w.data[j];
        }

        *b -= lr * grad_b;

        if (epoch % 10 == 0 || epoch == epochs - 1)
        {
            double loss = 0.0;
            if (mse(x, y_true, w, *b, &loss) < 0)
            {
                return -1;
            }
            printf("Epoch %d | Loss %.6f\n", epoch, loss);
        }
    }

    free(grad_w.data);
    return 0;
}