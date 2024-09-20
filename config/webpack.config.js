const path = require('path');
const webpack = require('webpack');
const HtmlWebpackPlugin = require('html-webpack-plugin');
const MiniCssExtractPlugin = require('mini-css-extract-plugin');
const TerserPlugin = require('terser-webpack-plugin');
const OptimizeCSSAssetsPlugin = require('optimize-css-assets-webpack-plugin');

// Set the environment
const isProduction = process.env.NODE_ENV === 'production';

module.exports = {
  // Define entry point
  entry: './src/index.ts',

  // Configure output
  output: {
    path: path.resolve(__dirname, 'dist'),
    filename: isProduction ? '[name].[contenthash].js' : '[name].js',
    clean: true,
  },

  // Set up module rules for TypeScript, CSS, and assets
  module: {
    rules: [
      {
        test: /\.tsx?$/,
        use: 'ts-loader',
        exclude: /node_modules/,
      },
      {
        test: /\.css$/,
        use: [
          isProduction ? MiniCssExtractPlugin.loader : 'style-loader',
          'css-loader',
        ],
      },
      {
        test: /\.(png|svg|jpg|jpeg|gif)$/i,
        type: 'asset/resource',
      },
    ],
  },

  // Configure plugins
  plugins: [
    new HtmlWebpackPlugin({
      template: './src/index.html',
    }),
    new MiniCssExtractPlugin({
      filename: isProduction ? '[name].[contenthash].css' : '[name].css',
    }),
    new webpack.DefinePlugin({
      'process.env.NODE_ENV': JSON.stringify(process.env.NODE_ENV || 'development'),
    }),
  ],

  // Set up optimization
  optimization: {
    minimizer: [
      new TerserPlugin(),
      new OptimizeCSSAssetsPlugin(),
    ],
    splitChunks: {
      chunks: 'all',
    },
  },

  // Configure dev server for development mode
  devServer: {
    contentBase: './dist',
    hot: true,
  },

  // Set mode based on environment
  mode: isProduction ? 'production' : 'development',

  // Enable source maps for debugging
  devtool: isProduction ? 'source-map' : 'inline-source-map',

  // Resolve extensions
  resolve: {
    extensions: ['.tsx', '.ts', '.js'],
  },
};

// Human tasks:
// TODO: Review and adjust optimization settings for production builds
// TODO: Implement code splitting for better performance
// TODO: Set up environment-specific configurations