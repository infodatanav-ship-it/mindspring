FROM php:7.3-apache

# Install system dependencies and PHP extensions required by the app (pdo_mysql, mysqli, gd, zip, mbstring, xml, exif)
RUN apt-get update \
  && apt-get install -y --no-install-recommends \
    libfreetype6-dev \
    libjpeg62-turbo-dev \
    libpng-dev \
    libzip-dev \
    zlib1g-dev \
    libxml2-dev \
    libonig-dev \
    unzip \
    git \
    curl \
    locales \
    fonts-dejavu-core \
  && docker-php-ext-configure gd --with-freetype-dir=/usr/include/ --with-jpeg-dir=/usr/include/ \
  && docker-php-ext-install -j$(nproc) gd mysqli pdo pdo_mysql zip mbstring exif xml \
  && a2enmod rewrite \
  && rm -rf /var/lib/apt/lists/*

# Install Composer globally
RUN curl -sS https://getcomposer.org/installer -o composer-setup.php \
  && php composer-setup.php --install-dir=/usr/local/bin --filename=composer \
  && rm composer-setup.php

# Copy a minimal php.ini
COPY docker/php.ini /usr/local/etc/php/php.ini

# Set working directory to the project root (mounted by compose)
WORKDIR /var/www/html

# Copy application files into the image so we can install Composer dependencies
COPY . /var/www/html

# Install PHP dependencies into the image if composer.json exists
RUN if [ -f composer.json ]; then \
      composer install --no-dev --prefer-dist --no-interaction --optimize-autoloader; \
    fi

# Ensure www-data owns the app files (compose mounts will override this at runtime)
RUN chown -R www-data:www-data /var/www/html || true

EXPOSE 80

CMD ["apache2-foreground"]
