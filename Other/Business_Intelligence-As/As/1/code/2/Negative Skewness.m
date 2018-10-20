X = -3:.1:3;
PDF = normpdf(X, 0, 0.8) + normpdf(X, 1.5, 0.9); % take the combination of two normal distrubtions
figure
plot(X, PDF); % plot the result
title('Negative Skewness: N(0, 0.8) + N(1.5, 0.9)')

PDF = PDF/sum(PDF); % normalise it, i.e. sum equals to one.
mean = 0;
for i = 1:length(X)
    mean = mean + PDF(i) * X(i);
end
mean = mean / length(X);

numerator = 0; 
denominator = 0;
for i = 1:length(X)
    numerator = numerator + PDF(i) * power(X(i) - mean, 3);
    denominator = denominator + PDF(i) * power(X(i) - mean, 2);
end
denominator = power(denominator / length(X), 1.5) * (length(X) - 1);

Skewness = numerator / denominator;
txt = sprintf('Skewness = %f', Skewness);
text(-2.5, 0.6, txt)