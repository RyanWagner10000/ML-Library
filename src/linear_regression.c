#include "../header/linear_regression.h"

int predict(Vector *x, Vector *w, double b, double *result)
{
    return dot_product(x, w, result);
}

int computeLoss(Matrix *x, Vector *y, Vector *w, double b, double *result, double lambda, bool regularize)
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

    if (regularize)
    {
        double reg_term = 0.0;
        for (int i = 0; i < w->size; ++i)
        {
            reg_term += w->data[i] * w->data[i];
        }
        *result += lambda * reg_term;
    }

    return 0;
}

int computeGradients(Matrix *x, Vector *y_true, Vector *w, double b, Vector *grad_w, double *grad_b, double lambda, bool regularize)
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

    // Average the gradients
    for (int j = 0; j < grad_w->size; ++j)
    {
        grad_w->data[j] /= x->rows;

        // Add regularization gradient
        if (regularize)
        {
            grad_w->data[j] += 2 * lambda * w->data[j];
        }
    }

    *grad_b /= x->rows;

    return 0;
}

int train_linear_model(Matrix *x, Vector *y_true, Vector *w, double *b, double lr, int epochs, double lambda, bool regularize)
{
    Vector grad_w = {malloc(sizeof(double) * w->size), w->size};
    double grad_b = 0.0;

    for (int epoch = 0; epoch < epochs; ++epoch)
    {
        if (computeGradients(x, y_true, w, *b, &grad_w, &grad_b, lambda, regularize) < 0)
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
            if (computeLoss(x, y_true, w, *b, &loss, lambda, regularize) < 0)
            {
                return -1;
            }
            printf("Epoch %d | Loss %.6f\n", epoch, loss);
        }
    }

    free(grad_w.data);
    return 0;
}