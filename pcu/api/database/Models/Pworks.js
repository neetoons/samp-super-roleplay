module.exports = (sequelize, DataTypes) => {
    return sequelize.define("pworks",
        {
            set: {
                type: DataTypes.INTEGER,
                allowNull: false,
                defaultValue: 0
            },
            level: {
                type: DataTypes.INTEGER,
                allowNull: false,
                defaultValue: 0
            }
        },
        {
            freezeTableName: true,
            timestamps: false
        }
    );
};